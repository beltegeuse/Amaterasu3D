#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneManager.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>

using namespace ama3D;

class ApplicationDeferred : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_gbuffer_shader;
	DeferredLighting* m_GI;
	FPS m_FPS;
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
		SettingsManager.SetProjection(1.0,4000.0,70.0);
		// Load shader
		m_gbuffer_shader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		// Load GI
		m_GI = new DeferredLighting();
		m_GI->SetFBOGraphicBuffer(m_gbuffer_shader->GetFBO());
		// Create light 1
//		PointLight light1;
//		light1.LightColor = Color(1.0,1.0,1.0,0.0);
//		light1.Position = Math::TVector3F(0,20,0);
//		light1.LightRaduis = 100.0;
//		light1.LightIntensity = 1.0;
//		m_GI->AddPointLight(light1);
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
		IMeshSceneNode* scene = SceneManager.LoadMesh("sponza.obj",0);
		SceneManager.AddScenegraphRoot(scene);

		Console.RegisterCommand("addlight",Console::Bind(&ApplicationDeferred::AddLight, *this));
	}

	void AddLight()
	{
		SpotLight light2;
		light2.LightColor = Color(1.0,1.0,1.0,0.0);
		light2.Position = m_Camera->GetPosition();
		light2.LightRaduis = 4000.0;
		light2.LightIntensity = 1.0;
		light2.LightCutOff = 70;
		light2.Direction = m_Camera->GetTarget();
		m_GI->AddSpotLight(light2);
	}

	virtual void OnUpdate(double delta)
	{}

	virtual void OnEvent(C3::Event& event)
	{
		if(event.Type == C3::Event::KeyPressed)
		{
			Math::CMatrix4 matrixTransform;
			 switch(event.Key.Code)
			 {
				 case C3::Key::F1:
					 m_debug = !m_debug;
					 break;
				 case C3::Key::F2:
					 m_GI->SetDebugMode(!m_GI->isDebugMode());
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_gbuffer_shader->Begin();
		m_Camera->GetView();
		SceneManager.RenderAll();
		m_gbuffer_shader->End();

		if(m_debug)
			m_gbuffer_shader->GetFBO()->DrawDebug();
		else
		{
			m_Camera->GetView();
			m_GI->ComputeIllumination();
		}

		MatrixManager.SetModeMatrix(MATRIX_2D);

		Console.Draw();
	}
};

#ifdef WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("../Donnees/Fonts/eve.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationDeferred window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
