#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/Model.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>
#include <Addons/Binvox/BinvoxModel.h>

class BinvoxViewer : public Application
{
protected:
	CameraFPS* m_Camera;
	FPS m_FPS;
	TShaderPtr m_BasicShader;
	bool m_debug;
public:
	BinvoxViewer() :
		m_debug(false)
	{
	}

	virtual ~BinvoxViewer()
	{
	}

	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Shaders
		m_BasicShader = ShaderManager.LoadShader("BasicShader.shader");
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(0.1,1000.0,70.0);
		// Load scene
		Logger::Log() << "[INFO] Load Armadillo.binvox\n";
		BinvoxModel model("Armadillo.binvox");
		RootSceneGraph.AddChild(model.CreateDebugPointModel());
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(SDL_Event& event)
	{
		if(event.type == SDL_KEYDOWN)
		{
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_debug = !m_debug;
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_BasicShader->Begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_BasicShader->End();

		MatrixManager.SetModeMatrix(MATRIX_2D);

		// Affichage du message d'aide
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
	BinvoxViewer window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
