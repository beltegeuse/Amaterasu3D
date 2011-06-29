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

class ApplicationGeom : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_GeomShader;
	FPS m_FPS;
	bool m_debug;
	float m_values[10];
public:
	ApplicationGeom() :
		m_debug(false)
	{

	}

	virtual ~ApplicationGeom()
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
		m_GeomShader = ShaderManager.LoadShader("TestUniformArrayShader.shader");
		for(int i = 0; i < 10; i++)
			m_values[i] = i*i / (100.0);
		// Load scene
		//		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
//		RootSceneGraph.AddChild(node1);
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
			 }
		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

//		m_Camera->GetView();
//		RootSceneGraph.Draw();
		//Set the clear color (black)
		glClearColor(0.0,0.0,0.0,1.0);
		//Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		//stretch to screen
		glViewport(0,0,800,600);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,800,0,600);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Draw a single line
		// it will stretch 1/2 the width and the full vertical
		//We will use a geometry shader to draw the same line, but with the x and y values swapped!
		// i.e. we will get a cross on the screen
		m_GeomShader->Begin();
		m_GeomShader->SetUniform1fv("values[0]", 10, m_values);
		glBegin(GL_LINES);
			glVertex2i(800/2, 0);
			glVertex2i(800/2, 600);
		glEnd();
		m_GeomShader->End();


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
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Impact.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationGeom window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
