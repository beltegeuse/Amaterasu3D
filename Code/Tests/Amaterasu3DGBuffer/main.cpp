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

class WindowGBuffer : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_gbuffer_shader;
	DeferredLighting* m_GI;
	bool m_debug;
public:
	WindowGBuffer() :
		Window("Amaterasu3DTestApp"),
		m_debug(false)
	{
		// Camera Setup
		CameraAbstract* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
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
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/Lighting/Deferred");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/2DShaders");
		// Load shader
		m_gbuffer_shader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		// Load GI
		m_GI = new DeferredLighting;
		m_GI->SetFBOGraphicBuffer(m_gbuffer_shader->GetFBO());
		// Create light 1
		PointLight light;
		light.LightColor = Color(1.0,1.0,1.0,0.0);
		light.Position = Math::TVector3F(3,4,2);
		light.LightRaduis = 5.0;
		light.LightIntensity = 1.0;
		m_GI->AddPointLight(light);
		// Load scene
		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.3DS");
		GetSceneRoot().AddChild(node1);
//		SceneGraph::AssimpNode* node2 = SceneGraph::AssimpNode::LoadFromFile("lion.obj");
//		SceneGraph::Group* nodeGroup = new SceneGraph::Group;
//		nodeGroup->AddChild(node2);
//		Math::CMatrix4 mat;
//		mat.SetTranslation(0,0,-1000.0);
//		nodeGroup->LoadTransformMatrix(mat);
//		GetSceneRoot().AddChild(nodeGroup);
	}

	virtual ~WindowGBuffer()
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
		m_gbuffer_shader->begin();
		//m_camera->SendInvMatrix();
		Window::OnDraw(delta);
		m_gbuffer_shader->end();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(m_debug)
			m_gbuffer_shader->GetFBO()->DrawDebug();
		else
		{
			m_camera->GetView();
			m_GI->ComputeIllumination();
		}

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowGBuffer window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
