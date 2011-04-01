#include <iostream>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <boost/random.hpp>

#include <Math/Matrix4.h>
#include <Application.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Model.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Graphics/Lighting/LightingStructures.h>
#include <Addons/FPS/FPS.h>
#include <Graphics/Shaders/Shader.h>

class ApplicationReflective : public Application
{
protected:
	FPS m_FPS;
	CameraFPS* m_Camera;
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	TShaderPtr m_RSMCompositing;

	Texture * m_textureRand;

	bool m_debug;
	bool m_debugGBuffer;
	bool m_debugCompositing;

	SpotLight m_light;
public:
	ApplicationReflective()
	{
	}

	virtual ~ApplicationReflective()
	{
	}

	void OnInitialize()
	{
		m_debug = false;
		m_debugGBuffer = false;
		m_debugCompositing = false;
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(10.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		CMatrixManager::Instance().SetProjectionMatrix(Math::CMatrix4::CreatePerspectiveFOV(70, (double)800/600, 0.1, 100));
		// Load shader
		m_GBufferShader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		m_RSMSpotShader = CShaderManager::Instance().LoadShader("RefectiveShadowMapSpot.shader");
		m_RSMCompositing = CShaderManager::Instance().LoadShader("RefectiveShadowMapCompositing.shader");
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
		SceneGraph::Model* lucyMesh = (SceneGraph::Model*)lucyModel->GetChilds()[0];
		Math::CMatrix4 lucyModelMatrix;
		lucyModelMatrix.SetScaling(1.0,1.0,1.0);
		lucyModel->LoadTransformMatrix(lucyModelMatrix);
		RootSceneGraph.AddChild(lucyModel);
		lucyMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("marble.jpg"));
		// * Scene loading
		SceneGraph::AssimpNode* sceneModel = SceneGraph::AssimpNode::LoadFromFile("uv_room.ply");
		SceneGraph::Model* sceneMesh = (SceneGraph::Model*)sceneModel->GetChilds()[0];
		sceneMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("bricks2_color.jpg"));
		Math::CMatrix4 sceneModelMatrix;
		sceneModelMatrix.SetScaling(5.0/3.0,5.0/3.0,5.0/3.0);
		sceneModel->LoadTransformMatrix(sceneModelMatrix);
		RootSceneGraph.AddChild(sceneModel);
		// Generate the texture
		GenerateRandomTexture(256);
	}

	virtual void OnUpdate(double deltaTime)
	{
	}

	void GenerateRandomTexture(int size, float rMax = 0.3)
	{
		boost::mt19937 rng;                 // produces randomness out of thin air
										  // see pseudo-random number generators
		boost::uniform_real<> range(0.0,1.0);       // distribution that maps to 1..6
										  // see random number distributions
		boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
			   die(rng, range);             // glues randomness with mapping
		m_textureRand = new Texture();
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
		Texture2DParams param;
		param.MinFiltering = GL_NEAREST;
		param.MaxFiltering = GL_NEAREST;
		param.applyParam();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size, size, 0, GL_RGBA, GL_FLOAT, tab);
		m_textureRand->desactivateTextureMapping();
	}

	virtual void OnEvent(SDL_Event& event)
	{
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

	virtual void OnRender()
	{

		// ========== Second STEPS (RSM generation buffers)
		// Fill in RSM spot buffers
		// * Matrix Setup
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_light.Position, m_light.Direction);
		Math::CMatrix4 LightProjectionMatrix = Math::CMatrix4::CreatePerspectiveFOV(m_light.LightCutOff,800.0/600.0, 1.0, m_light.LightRaduis);
		Math::CMatrix4 oldProjectionMatrix;
		Math::CMatrix4 oldViewMatrix;
		// * Save old transformations
		oldProjectionMatrix = CMatrixManager::Instance().GetMatrix(PROJECTION_MATRIX);
		oldViewMatrix = CMatrixManager::Instance().GetMatrix(VIEW_MATRIX);
		// * Go to the camera view
		CMatrixManager::Instance().SetProjectionMatrix(LightProjectionMatrix);
		CMatrixManager::Instance().SetViewMatrix(LightViewMatrix);
		// * Enable Shader
		m_RSMSpotShader->Begin();
		// *** Send all Uniform values
		m_RSMSpotShader->SetUniform1f("LightRaduis",m_light.LightRaduis);
		m_RSMSpotShader->SetUniform1f("LightCutOff", cos(m_light.LightCutOff *(M_PI / 180.0)));
		m_RSMSpotShader->SetUniform1f("LightIntensity", m_light.LightIntensity);
		m_RSMSpotShader->SetUniformVector("LightPosition", m_light.Position);
		m_RSMSpotShader->SetUniformVector("LightSpotDirection", m_light.Direction);
		m_RSMSpotShader->SetUniformColor("LightColor", m_light.LightColor);
		// * Draw the scene
		RootSceneGraph.Draw();
		m_RSMSpotShader->End();
		// * Revert transformations
		CMatrixManager::Instance().SetProjectionMatrix(oldProjectionMatrix);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		// =========== First STEPS (GBuffer generation)
		// Fill in the GBuffer
		m_GBufferShader->Begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_GBufferShader->End();

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
		m_RSMCompositing->Begin();
		// *** Send all Uniform values
		m_RSMCompositing->SetUniform1f("LightRaduis",m_light.LightRaduis);
		m_RSMCompositing->SetUniform1f("LightCutOff", cos(m_light.LightCutOff *(M_PI / 180.0)));
		m_RSMCompositing->SetUniform1f("LightIntensity", m_light.LightIntensity);
		m_RSMCompositing->SetUniformVector("LightPosition", m_light.Position);
		m_RSMCompositing->SetUniformVector("LightSpotDirection", m_light.Direction);
		m_RSMCompositing->SetUniformColor("LightColor", m_light.LightColor);
		m_RSMCompositing->SetUniform1i("DebugMode", m_debugCompositing);

		m_RSMCompositing->SetUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
		m_RSMCompositing->SetUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
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
		m_RSMCompositing->End();
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

		Console.Draw();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;
	ApplicationReflective window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
