#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <Math/Matrix4.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Graphics/SceneNode/SimpleRenderable/Rectangle2D.h>
#include <Graphics/Font/FontManager.h>
#include <Addons/FPS/FPS.h>
#include <tinyxml.h>
#include <TinyXMLHelper.h>
#include <Debug/Exceptions.h>

class TestCustomAttributs : public ama3D::Application
{
protected:
	/*
	 * Attributes
	 */
	ama3D::CameraFPS* m_Camera;
	ama3D::TShaderPtr m_ShaderTest;
	ama3D::Rectangle2D* m_Plane;
	std::vector<ama3D::CGraphicString> m_Texts;
public:
	TestCustomAttributs()
	{
	}

	virtual ~TestCustomAttributs()
	{
	}

	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new ama3D::CameraFPS(ama3D::Math::TVector3F(30,40,20), ama3D::Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);

		m_Texts.push_back(ama3D::CGraphicString(ama3D::Math::TVector2F(10,10),"Valve Font system !"));
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
		if(event.Type == C3::Event::KeyPressed)
		{
			 switch(event.Key.Code)
			 {
				 case C3::Key::F1:
					 glEnable(GL_ALPHA_TEST);
					 break;
				 case C3::Key::F2:
					 glDisable(GL_ALPHA_TEST);
					 break;
			 }
		}
	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(ama3D::MATRIX_3D);

		MatrixManager.SetModeMatrix(ama3D::MATRIX_2D);

		for(int i = 0; i < m_Texts.size(); i++)
			m_Texts[i].Draw();

	}
};

//#ifdef WIN32
//#include <windows.h>
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//#else
int main(int argc, char *argv[])
//#endif
{

	ama3D::CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	ama3D::CFontManager::Instance().LoadFont("Cheeseburger_ttf.font");
	std::cout << "[INFO] Begin ..." << std::endl;
	TestCustomAttributs window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;


	return 0;
}
