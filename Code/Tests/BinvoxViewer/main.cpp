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
#include <Addons/FPS/FPS.h>
#include <Addons/Binvox/BinvoxModel.h>

using namespace ama3D;

class BinvoxViewer : public Application
{
protected:
	CameraFPS* m_Camera;
	FPS m_FPS;

	BinvoxModel* m_BinVox;
public:
	BinvoxViewer()
	{
	}

	virtual ~BinvoxViewer()
	{
	}

	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(30,40,20), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(0.1,1000.0,70.0);
		// Load scene
		Logger::Log() << "[INFO] Load Armadillo.binvox\n";
		m_BinVox = new BinvoxModel("Dragon.binvox");
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
//		if(event.Type == C3::Event::KeyPressed)
//		{
//			 switch(event.Key.Code)
//			 {
//			 }
//		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_BinVox->UpdateCubeFBO(m_Camera);
		m_BinVox->Render();

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
