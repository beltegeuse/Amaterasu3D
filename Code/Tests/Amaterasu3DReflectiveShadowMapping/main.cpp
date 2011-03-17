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
#include <Graphics/MatrixManagement.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class WindowReflective : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	bool m_debug;
	SpotLight m_light;
public:
	WindowReflective() :
		Window("Amaterasu3DTestApp"),
		m_debug(false)
	{
		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		cam->SetSpeed(10.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 0.1, 100);
		MatrixManagement::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Config path
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/SponzaOther");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza/textures");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/GBuffers");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/Lighting/RSM");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/2DShaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/Shadow");
		// Load shader
		m_GBufferShader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		m_RSMSpotShader = glShaderManager::Instance().LoadShader("RefectiveShadowMapSpot.shader");
		// Create light
		m_light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_light.Position = Math::TVector3F(0,5,10);
		m_light.LightRaduis = 100.0;
		m_light.LightIntensity = 1.0;
		m_light.LightCutOff = 70;
		m_light.Direction = Math::TVector3F(0.0,-0.3,-0.7);
		m_light.Direction.Normalize();
		// Load scene
		// * Lucy loading
		SceneGraph::AssimpNode* lucyModel = SceneGraph::AssimpNode::LoadFromFile("uv_lucy.ply");
		SceneGraph::AssimpMesh* lucyMesh = (SceneGraph::AssimpMesh*)lucyModel->GetChilds()[0];
		Math::CMatrix4 lucyModelMatrix;
		lucyModelMatrix.SetScaling(3.0,3.0,3.0);
		lucyModel->LoadTransformMatrix(lucyModelMatrix);
		GetSceneRoot().AddChild(lucyModel);
		lucyMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("marble.jpg"));
		// * Scene loading
		SceneGraph::AssimpNode* sceneModel = SceneGraph::AssimpNode::LoadFromFile("uv_room.ply");
		SceneGraph::AssimpMesh* sceneMesh = (SceneGraph::AssimpMesh*)sceneModel->GetChilds()[0];
		sceneMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("bricks2_color.jpg"));
		Math::CMatrix4 sceneModelMatrix;
		sceneModelMatrix.SetScaling(5.0,5.0,5.0);
		sceneModel->LoadTransformMatrix(sceneModelMatrix);
		GetSceneRoot().AddChild(sceneModel);

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
			 }
		}

	}

	virtual void OnDraw(double delta)
	{
		// Fill in the GBuffer
		m_GBufferShader->begin();
		Window::OnDraw(delta);
		m_GBufferShader->end();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_RSMSpotShader->GetFBO()->DrawDebug();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowReflective window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
