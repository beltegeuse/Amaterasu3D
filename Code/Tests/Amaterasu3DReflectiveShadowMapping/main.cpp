#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/Camera/CameraFly.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <System/SettingsManager.h>
#include <Graphics/Font/FontManager.h>
#include <Graphics/MatrixManagement.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <boost/random.hpp>

class WindowReflective : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	TShaderPtr m_RSMCompositing;
	Texture * m_textureRand;
	bool m_debug;
	bool m_debugGBuffer;
	bool m_debugCompositing;
	SpotLight m_light;
public:
	WindowReflective() :
		Window("Amaterasu3DTestApp"),
		m_debug(false),
		m_debugGBuffer(false),
		m_debugCompositing(false)
	{
		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		cam->SetSpeed(10.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 0.1, 100);
		MatrixManagement::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Load shader
		m_GBufferShader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		m_RSMSpotShader = glShaderManager::Instance().LoadShader("RefectiveShadowMapSpot.shader");
		m_RSMCompositing = glShaderManager::Instance().LoadShader("RefectiveShadowMapCompositing.shader");
		// Create light
		m_light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_light.Position = Math::TVector3F(0,10.0/3.0,6.0);
		m_light.LightRaduis = 100.0;
		m_light.LightIntensity = 1.0;
		m_light.LightCutOff = 50;
		m_light.Direction = Math::TVector3F(0.0,-0.6,-1.4);
		m_light.Direction.Normalize();
		// Load scene
		// * Lucy loading
		SceneGraph::AssimpNode* lucyModel = SceneGraph::AssimpNode::LoadFromFile("uv_lucy.ply");
		SceneGraph::AssimpMesh* lucyMesh = (SceneGraph::AssimpMesh*)lucyModel->GetChilds()[0];
		Math::CMatrix4 lucyModelMatrix;
		lucyModelMatrix.SetScaling(1.0,1.0,1.0);
		lucyModel->LoadTransformMatrix(lucyModelMatrix);
		GetSceneRoot().AddChild(lucyModel);
		lucyMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("marble.jpg"));
		// * Scene loading
		SceneGraph::AssimpNode* sceneModel = SceneGraph::AssimpNode::LoadFromFile("uv_room.ply");
		SceneGraph::AssimpMesh* sceneMesh = (SceneGraph::AssimpMesh*)sceneModel->GetChilds()[0];
		sceneMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("bricks2_color.jpg"));
		Math::CMatrix4 sceneModelMatrix;
		sceneModelMatrix.SetScaling(5.0/3.0,5.0/3.0,5.0/3.0);
		sceneModel->LoadTransformMatrix(sceneModelMatrix);
		GetSceneRoot().AddChild(sceneModel);
		// Generate the texture
		GenerateRandomTexture(256);
	}

	void GenerateRandomTexture(int size, float rMax = 0.3)
	{
		boost::mt19937 rng;                 // produces randomness out of thin air
										  // see pseudo-random number generators
		boost::uniform_real<> range(0.0,1.0);       // distribution that maps to 1..6
										  // see random number distributions
		boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
			   die(rng, range);             // glues randomness with mapping
		m_textureRand = new Texture(Math::TVector2I(size,size));
		float * tab = new float[size*size*4];
		for(int i = 0; i < size*size; i++)
		{
			// Get two random number
			float a = die();
			float b = die();
			tab[4*i] = rMax*a*sin(2*M_PI*b);
			tab[4*i+1] = rMax*a*cos(2*M_PI*b);
			//std::cout << tab[4*i] << " " << tab[4*i+1] << std::endl;
			tab[4*i+2] = a*a;
			tab[4*i+3] = 1.0;
		}
		m_textureRand->activateTextureMapping();
		m_textureRand->activateTexture();
		TextureParams param;
		param.MinFiltering = GL_NEAREST;
		param.MaxFiltering = GL_NEAREST;
		param.applyParam();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size, size, 0, GL_RGBA, GL_FLOAT, tab);
		m_textureRand->desactivateTextureMapping();
	}

	virtual ~WindowReflective()
	{
	}

	virtual void OnEvent(SDL_Event& event, double delta)
	{
		Window::OnEvent(event, delta);
		if(event.type == SDL_KEYDOWN)
		{
			Math::CMatrix4 matrixTransform;
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_debug = !m_debug;
					 break;
				 case SDLK_F2:
					 m_debugGBuffer = !m_debugGBuffer;
					 break;
				 case SDLK_F3:
					 m_debugCompositing = !m_debugCompositing;
					 break;
			 }
		}

	}

	virtual void OnDraw(double delta)
	{
		// =========== First STEPS (GBuffer generation)
		// Fill in the GBuffer
		m_GBufferShader->begin();
		Window::OnDraw(delta);
		m_GBufferShader->end();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ========== Second STEPS (RSM generation buffers)
		// Fill in RSM spot buffers
		// * Matrix Setup
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_light.Position, m_light.Direction);
		Math::CMatrix4 LightProjectionMatrix;
		LightProjectionMatrix.PerspectiveFOV(m_light.LightCutOff, 800.0/600.0, 1.0, m_light.LightRaduis);
		Math::CMatrix4 oldProjectionMatrix;
		Math::CMatrix4 oldViewMatrix;
		// * Save old transformations
		oldProjectionMatrix = MatrixManagement::Instance().GetMatrix(PROJECTION_MATRIX);
		oldViewMatrix = MatrixManagement::Instance().GetMatrix(VIEW_MATRIX);
		// * Go to the camera view
		MatrixManagement::Instance().SetProjectionMatrix(LightProjectionMatrix);
		MatrixManagement::Instance().SetViewMatrix(LightViewMatrix);
		// * Enable Shader
		m_RSMSpotShader->begin();
		// *** Send all Uniform values
		m_RSMSpotShader->setUniform1f("LightRaduis",m_light.LightRaduis);
		m_RSMSpotShader->setUniform1f("LightCutOff", cos(m_light.LightCutOff *(M_PI / 180.0)));
		m_RSMSpotShader->setUniform1f("LightIntensity", m_light.LightIntensity);
		m_RSMSpotShader->setUniform3f("LightPosition", m_light.Position.x, m_light.Position.y, m_light.Position.z);
		m_RSMSpotShader->setUniform3f("LightSpotDirection", m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);
		m_RSMSpotShader->setUniform3f("LightColor", m_light.LightColor.R, m_light.LightColor.G, m_light.LightColor.B);
		// * Draw the scene
		GetSceneRoot().Draw();
		m_RSMSpotShader->end();
		// * Revert transformations
		MatrixManagement::Instance().SetProjectionMatrix(oldProjectionMatrix);
		MatrixManagement::Instance().SetViewMatrix(oldViewMatrix);

		// ========= Third STEPS : Compositing
		// Bind all texture location
		m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_GBufferShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+3);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+4);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+5);
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+6);
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+7);
		m_textureRand->activateMultiTex(CUSTOM_TEXTURE+8);
		m_RSMCompositing->begin();
		// *** Send all Uniform values
		m_RSMCompositing->setUniform1f("LightRaduis",m_light.LightRaduis);
		m_RSMCompositing->setUniform1f("LightCutOff", cos(m_light.LightCutOff *(M_PI / 180.0)));
		m_RSMCompositing->setUniform1f("LightIntensity", m_light.LightIntensity);
		m_RSMCompositing->setUniform3f("LightPosition", m_light.Position.x, m_light.Position.y, m_light.Position.z);
		m_RSMCompositing->setUniform3f("LightSpotDirection", m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);
		m_RSMCompositing->setUniform3f("LightColor", m_light.LightColor.R, m_light.LightColor.G, m_light.LightColor.B);
		m_RSMCompositing->setUniform1i("DebugMode", m_debugCompositing);

		m_RSMCompositing->setUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
		m_RSMCompositing->setUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
		// Draw Quad
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-1.0, -1.0);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-1.0, 1.0);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_RSMCompositing->end();
		// Desactivate all texture unit
		m_GBufferShader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_GBufferShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+3);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+4);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+5);
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+6);
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+7);
		m_textureRand->desactivateMultiTex(CUSTOM_TEXTURE+8);

		if(m_debug)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_RSMSpotShader->GetFBO()->DrawDebug();
		}
		else if(m_debugGBuffer)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_GBufferShader->GetFBO()->DrawDebug();
		}

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	// FIXME: Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowReflective window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
