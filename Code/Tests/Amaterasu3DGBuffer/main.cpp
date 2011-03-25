#include <iostream>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>

class ApplicationDeferred : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_gbuffer_shader;
	DeferredLighting* m_GI;
	bool m_debug;
public:
	ApplicationDeferred() :
		m_debug(false)
	{

	}

	virtual ~ApplicationDeferred()
	{
	}

	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(200.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		CMatrixManager::Instance().SetProjectionMatrix(Math::CMatrix4::PerspectiveFOV(70, (double)800/600, 0.1, 4000));
		// Load shader
		m_gbuffer_shader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		// Load GI
		m_GI = new DeferredLighting(RootSceneGraph);
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
		RootSceneGraph.AddChild(node1);
	}

	virtual void OnUpdate(double delta)
	{}

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
					 m_GI->SetDebugMode(!m_GI->isDebugMode());
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		m_gbuffer_shader->begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
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
			m_Camera->GetView();
			m_GI->ComputeIllumination();
		}

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		Console.Draw();
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationDeferred window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
