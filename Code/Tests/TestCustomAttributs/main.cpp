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
#include <Addons/FPS/FPS.h>


class TestCustomAttributs : public ama3D::Application
{
protected:
	/*
	 * Attributes
	 */
	ama3D::CameraFPS* m_Camera;
	ama3D::TShaderPtr m_ShaderTest;
	ama3D::ISimpleRenderableSceneNode * m_RenderableObj;
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
		// Shader loading ...
		m_ShaderTest = ShaderManager.LoadShader("TestCustomAttribute.shader");
		// Create buffer
		ama3D::Math::TVector2I sizeWindow = SettingsManager.GetSizeRenderingWindow();
		int nbElements = sizeWindow.x*sizeWindow.y;
		// Create Data
		float * CustomPosition = new float[nbElements*2];
		float * CustomColor = new float[nbElements*3];

		boost::minstd_rand generator(42u);
		boost::uniform_real<> uni_dist(0,1);
		boost::variate_generator<boost::minstd_rand, boost::uniform_real<> > uni(generator, uni_dist);

		for(int i = 0; i < sizeWindow.x; i++)
			for(int j = 0; j < sizeWindow.y; j++)
			{
				int indiceData = (i*sizeWindow.y+j);
				CustomPosition[indiceData*2] = i;
				CustomPosition[indiceData*2+1] = j;
				CustomColor[indiceData*3] = uni();
				CustomColor[indiceData*3+1] = uni();
				CustomColor[indiceData*3+2] = uni();
			}

		ama3D::RenderableObject::RenderableBuffer bufferPosition;
		ama3D::RenderableObject::RenderableBuffer bufferColor;
		// Configure
		// * Position
		bufferPosition.buffer = CustomPosition;
		bufferPosition.dimension = 2;
		bufferPosition.owner = true;
		bufferPosition.size = nbElements*2;
		// * Color
		bufferColor.buffer = CustomColor;
		bufferColor.dimension = 3;
		bufferColor.owner = true;
		bufferColor.size = nbElements*3;
		// Create indice buffer
		unsigned int* indiceBuffer = new unsigned int[nbElements];
		for(int i = 0; i < nbElements; i++)
			indiceBuffer[i] = i;
		// Create object and configure
		m_RenderableObj = new ama3D::ISimpleRenderableSceneNode("TestObject", 0);
		m_RenderableObj->GetObject().SetDrawMode(GL_POINTS);
		m_RenderableObj->GetObject().SetIndiceBuffer(indiceBuffer, nbElements);
		m_RenderableObj->GetObject().AddBuffer(bufferPosition, ama3D::CUSTOM_ATTRIBUT+0);
		m_RenderableObj->GetObject().AddBuffer(bufferColor, ama3D::CUSTOM_ATTRIBUT+1);
		m_RenderableObj->GetObject().CompileBuffers();
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(ama3D::MATRIX_3D);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_ShaderTest->Begin();
		ama3D::Math::TVector2I sizeWindow = SettingsManager.GetSizeRenderingWindow();
		m_ShaderTest->SetUniformVector("ScreenSize",ama3D::Math::TVector2F(sizeWindow.x,sizeWindow.y));
		m_RenderableObj->Render();
		m_ShaderTest->End();

		MatrixManager.SetModeMatrix(ama3D::MATRIX_2D);

		// Affichage du message d'aide
		//Console.Draw();

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
