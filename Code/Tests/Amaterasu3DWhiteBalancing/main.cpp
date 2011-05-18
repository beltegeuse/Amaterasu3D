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

struct FBOCube
{
	FBO* Fbo;
	Math::CMatrix4 TransMatrix;
};

class ApplicationWhite : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_ParaboloidShader;
	TShaderPtr m_CubeShader;
	FPS m_FPS;
	FBOCube* m_CubeFBOs;
	bool m_debug;
	bool m_ParaboloidDraw;
public:
	ApplicationWhite() :
		m_debug(false),
		m_ParaboloidDraw(false)
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
		m_CubeShader = CShaderManager::Instance().LoadShader("CubeProjection.shader");
		// Creation des autres buffers
		GenerateCubeBuffers();
		// Load scene
		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
		RootSceneGraph.AddChild(node1);
	}

	void GenerateCubeBuffers()
	{
		m_CubeFBOs = new FBOCube[5];

		// Initialisation front face
		m_CubeFBOs[0].Fbo = m_CubeShader->GetFBO();
		m_CubeFBOs[0].TransMatrix.Identity();

		// Set rotation
		m_CubeFBOs[1].TransMatrix.SetRotationX(90);
		m_CubeFBOs[2].TransMatrix.SetRotationX(-90);
		m_CubeFBOs[3].TransMatrix.SetRotationY(90);
		m_CubeFBOs[4].TransMatrix.SetRotationY(-90);

		// Compute all
		Math::CMatrix4 trans;
		trans.SetTranslation(0.5,0.0,0.5);
		for(int i = 1; i < 5; i++)
		{
			m_CubeFBOs[i].Fbo = m_CubeFBOs[0].Fbo->Copy();
			m_CubeFBOs[i].TransMatrix = trans*m_CubeFBOs[i].TransMatrix;
		}
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
					 m_ParaboloidDraw = !m_ParaboloidDraw;
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_Camera->GetView();
		if(m_ParaboloidDraw)
		{
			m_ParaboloidShader->Begin();
			m_ParaboloidShader->SetUniform1f("FarClipping",4000.0f);
			m_ParaboloidShader->SetUniform1f("NearClipping",1.0f);
			RootSceneGraph.Draw();
			m_ParaboloidShader->End();
		}
		else
		{
			for(int i = 0; i < 5; i++)
			{
				m_CubeShader->SetFBO(m_CubeFBOs[i].Fbo, false);
				m_CubeShader->Begin();
				m_CubeShader->SetUniformMatrix4fv("TransformMatrix", m_CubeFBOs[i].TransMatrix);
				RootSceneGraph.Draw();
				m_CubeShader->End();
			}

			m_CubeShader->GetFBO()->DrawDebug();
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
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationWhite window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
