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
#include <Graphics/Font/GraphicString.h>
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
	CGraphicString m_Message;
	bool m_debug;
public:
	WindowGBuffer() :
		Window("Amaterasu3DTestApp"),
		m_debug(false)
	{
		// Message d'aide
		m_Message.Position = Math::TVector2F(5, 577);
		m_Message.Text     = "F1 / F2 pour afficher / cacher la console | echap pour quitter";
		m_Message.Color    = CColor(255, 255, 255, 100);
		m_Message.Size     = 18;

		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		cam->SetSpeed(200.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 0.1, 4000);
		MatrixManagement::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Config path
		CMediaManager::Instance().AddSearchPathAndChilds("../Donnees");
		// Load shader
		m_gbuffer_shader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		// Load GI
		m_GI = new DeferredLighting(this);
		m_GI->SetFBOGraphicBuffer(m_gbuffer_shader->GetFBO());
		// Create light 1
		PointLight light1;
		light1.LightColor = Color(1.0,1.0,1.0,0.0);
		light1.Position = Math::TVector3F(0,20,0);
		light1.LightRaduis = 100.0;
		light1.LightIntensity = 1.0;
		m_GI->AddPointLight(light1);
		// Create light 2
		SpotLight light2;
		light2.LightColor = Color(1.0,1.0,1.0,0.0);
		light2.Position = Math::TVector3F(-500,200,0);
		light2.LightRaduis = 4000.0;
		light2.LightIntensity = 1.0;
		light2.LightCutOff = 70;
		light2.Direction = Math::TVector3F(1.0,0.0,0.0);
		m_GI->AddSpotLight(light2);
		// Load scene
		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
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
				 case SDLK_F2:
					 m_GI->SetDebugMode(!m_GI->isDebugMode());
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

		// Affichage du message d'aide
		m_Message.Draw();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
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
