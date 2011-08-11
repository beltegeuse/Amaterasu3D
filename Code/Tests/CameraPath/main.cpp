#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Graphics/Camera/FixedCamera.h>
#include <Addons/FPS/FPS.h>

#include <Math/Quaternion.h>
#include <Math/Vector3.h>
#include <Math/Interpolators/CatmullRomInterpolator.h>
#include <Animation/CameraAnimation.h>

using namespace ama3D;

class ApplicationCamera : public Application
{
protected:
	CameraFPS* m_Camera;
	CameraAnimation* m_CameraAnimation;
	TShaderPtr m_gbuffer_shader;
	DeferredLighting* m_GI;
	FPS m_FPS;
	bool m_debug;
	bool m_play;
public:
	/*
	 * Console methods
	 */
	std::string ConsoleAddControlPoint(float deltaTime)
	{
		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(m_Camera->GetPosition(), m_Camera->GetTarget() - m_Camera->GetPosition()), deltaTime);
		std::stringstream ss;
		ss << "Add point : " << m_Camera->GetPosition() << " | " << m_Camera->GetTarget() - m_Camera->GetPosition() << " ( " << deltaTime << ")";
		return ss.str();
	}

	void ConsoleCompilePlay()
	{
		m_play = true;
		m_CameraAnimation->Compile();
	}

	void ConsoleStop()
	{
		m_play = false;
	}

	void ConsoleEraseAll()
	{
		m_CameraAnimation->EraseAllControlPoints();
	}

	void ConsoleWrite(const std::string& path)
	{
		std::string fullpath = path+".xml";
		m_CameraAnimation->WriteXMLFile(fullpath);
	}

	void ConsoleRead(const std::string& path)
	{
		std::string fullpath = path+".xml";
		m_CameraAnimation->ReadXMLFile(fullpath);
	}

	/*
	 * Constructor and destructors
	 */
	ApplicationCamera() :
		m_debug(false),
		m_play(false)
	{

	}

	virtual ~ApplicationCamera()
	{
	}

	/*
	 * Other methods
	 */
	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(200.0);
		m_CameraAnimation = new CameraAnimation(new FixedCamera(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0)));
		AddControlPoints();
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(1.0,4000.0,70.0);
		// Load shader
		m_gbuffer_shader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		// Load GI
		m_GI = new DeferredLighting();
		m_GI->SetFBOGraphicBuffer(m_gbuffer_shader->GetFBO());
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
		IMeshSceneNode* node1 = SceneManager.LoadMesh("sponza.obj");
		SceneManager.AddScenegraphRoot(node1);

		// Add Console functions
		Console.RegisterCommand("add",Console::Bind(&ApplicationCamera::ConsoleAddControlPoint, *this));
		Console.RegisterCommand("compile",Console::Bind(&ApplicationCamera::ConsoleCompilePlay, *this));
		Console.RegisterCommand("stop",Console::Bind(&ApplicationCamera::ConsoleStop, *this));
		Console.RegisterCommand("erase",Console::Bind(&ApplicationCamera::ConsoleEraseAll, *this));
		Console.RegisterCommand("write",Console::Bind(&ApplicationCamera::ConsoleWrite, *this));
		Console.RegisterCommand("read",Console::Bind(&ApplicationCamera::ConsoleRead, *this));
	}

	void AddControlPoints()
	{
//		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,1,0)),10);
//		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,-1,0)),10);
		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(1,0,0)),0);
		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(-1,0,0)),10);
		//m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,0,1)),10);
		//m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,0,-1)),10);

	}

	virtual void OnUpdate(double delta)
	{
		if(m_play)
			m_CameraAnimation->Update(delta);
	}

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

		if(!m_play)
			m_Camera->GetView();
		else
			m_CameraAnimation->GetCamera()->GetView();

		SceneManager.RenderAll();
		m_gbuffer_shader->End();

		if(m_debug)
			m_gbuffer_shader->GetFBO()->DrawDebug();
		else
		{
			if(!m_play)
				m_Camera->GetView();
			else
				m_CameraAnimation->GetCamera()->GetView();

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
	CFontManager::Instance().LoadFont("Cheeseburger_ttf.font");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationCamera window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
