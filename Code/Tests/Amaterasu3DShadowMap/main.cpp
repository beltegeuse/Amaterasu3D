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

class WindowShadow : public Window
{
protected:
	TShaderPtr m_BasicShader;
	TShaderPtr m_ShadowShader;
	Math::CMatrix4 m_matrixPerspective;
	SpotLight m_light;

	bool m_debug;
public:
	WindowShadow() :
		Window("WindowShadow"),
		m_debug(false)
	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		cam->SetSpeed(20.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 0.1, 4000);
		MatrixManagement::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Config path
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/SponzaOther");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza/textures");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/2DShaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/GBuffers");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/Lighting/Deferred");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/BasicShaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/Shadow");
		// Shader loading
		m_BasicShader = glShaderManager::Instance().LoadShader("BasicShader.shader");
		m_ShadowShader = glShaderManager::Instance().LoadShader("ShadowMap.shader");

		// Create Light
		m_light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_light.Position = Math::TVector3F(0,10,0);
		m_light.LightRaduis = 40.0;
		m_light.LightIntensity = 1.0;
		m_light.LightCutOff = 70;
		m_light.Direction = Math::TVector3F(4.0,4.0,4.0);
		// Load scene
		// * Create first cube
		SceneGraph::Group * cubeGroup = new SceneGraph::Group;
		cubeGroup->AddChild(new DebugCubeLeaf);
		Math::CMatrix4 matCube;
		matCube.SetTranslation(4,4,4);
		cubeGroup->LoadTransformMatrix(matCube);
		// * Create floor
		SceneGraph::Group * cubeFloor = new SceneGraph::Group;
		cubeFloor->AddChild(new DebugCubeLeaf);
		Math::CMatrix4 matFloor;
		matFloor.SetTranslation(0,0,0);
		matFloor.SetScaling(10,1,10);
		cubeFloor->LoadTransformMatrix(matFloor);
		// Add to root
		GetSceneRoot().AddChild(cubeGroup);
		GetSceneRoot().AddChild(cubeFloor);
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
		m_BasicShader->begin();
		Window::OnDraw(delta);
		m_BasicShader->end();
//
//		glMatrixMode(GL_PROJECTION);
//		glPushMatrix();
//		glLoadIdentity();
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glLoadIdentity();
//
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		glPopMatrix();
//		glMatrixMode(GL_PROJECTION);
//		glPopMatrix();
//		glMatrixMode(GL_MODELVIEW);

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowShadow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
