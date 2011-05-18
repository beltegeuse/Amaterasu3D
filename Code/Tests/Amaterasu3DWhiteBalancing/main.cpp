#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Model.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>

class ApplicationWhite : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_ParaboloidShader;
	//DeferredLighting* m_GI;
	FPS m_FPS;
	bool m_debug;
public:
	ApplicationWhite() :
		m_debug(false)
	{

	}

	virtual ~ApplicationWhite()
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
		m_ParaboloidShader = CShaderManager::Instance().LoadShader("ParaboloidProjection.shader");
		// Create light 1
//		PointLight light1;
//		light1.LightColor = Color(1.0,1.0,1.0,0.0);
//		light1.Position = Math::TVector3F(0,20,0);
//		light1.LightRaduis = 100.0;
//		light1.LightIntensity = 1.0;
//		m_GI->AddPointLight(light1);
		// Create light 2
//		SpotLight light2;
//		light2.LightColor = Color(1.0,1.0,1.0,0.0);
//		light2.Position = Math::TVector3F(-500,200,0);
//		light2.LightRaduis = 4000.0;
//		light2.LightIntensity = 1.0;
//		light2.LightCutOff = 70;
//		light2.Direction = Math::TVector3F(1.0,0.0,0.0);
//		m_GI->AddSpotLight(light2);
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
//					 m_GI->SetDebugMode(!m_GI->isDebugMode());
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_ParaboloidShader->Begin();
		m_ParaboloidShader->SetUniform1f("FarClipping",4000.0f);
		m_ParaboloidShader->SetUniform1f("NearClipping",1.0f);
		RootSceneGraph.Draw();
		m_ParaboloidShader->End();

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
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationWhite window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
