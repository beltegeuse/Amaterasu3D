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
#include <Graphics/SceneGraph/Rectangle2D.h>

struct FBOCube
{
	FBO* Fbo;
	Math::CMatrix4 ProjectionMatrix;
	Math::CMatrix4 TransMatrix;
	Rectangle2D* Model;
};

class ApplicationWhite : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_ParaboloidShader;
	TShaderPtr m_CubeShader;
	TShaderPtr m_2DDraw;
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
		m_2DDraw = CShaderManager::Instance().LoadShader("2DDraw.shader");
		// Creation des autres buffers
		GenerateCubeBuffers();
		// Load scene
		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
		RootSceneGraph.AddChild(node1);
	}

	void GenerateCubeBuffers(float Far = 4000.0)
	{
		float Near = 1.0f;
		m_CubeFBOs = new FBOCube[5];

		// Initialisation front face
		m_CubeFBOs[0].Fbo = m_CubeShader->GetFBO();
		m_CubeFBOs[0].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,-1),Math::TVector2F(1,1),Near, Far);
		m_CubeFBOs[0].TransMatrix.Identity();

		// Set rotation
		m_CubeFBOs[1].TransMatrix.SetRotationX(M_PI/2.0);
		m_CubeFBOs[2].TransMatrix.SetRotationX(-M_PI/2.0);
		m_CubeFBOs[3].TransMatrix.SetRotationY(-M_PI/2.0);
		m_CubeFBOs[4].TransMatrix.SetRotationY(M_PI/2.0);

		// Set model for debug
		for(int i = 0; i < 5; i++)
			m_CubeFBOs[i].Model = 0;
		Math::TVector2I size = SettingsManager.GetSizeRenderingWindow();
		m_CubeFBOs[0].Model = new Rectangle2D(Math::TVector2I(0.25*size.x,0.75*size.y),Math::TVector2I(0.75*size.x,0.25*size.y));
		m_CubeFBOs[0].Model->AddTextureMap(DIFFUSE_TEXTURE, m_CubeFBOs[0].Fbo->GetTexture("ColorBuffer"));
		m_CubeFBOs[1].Model = new Rectangle2D(Math::TVector2I(0.25*size.x,0.25*size.y),Math::TVector2I(0.75*size.x,0.0*size.y));
		m_CubeFBOs[2].Model = new Rectangle2D(Math::TVector2I(0.25*size.x,1.0*size.y),Math::TVector2I(0.75*size.x,0.75*size.y));
		m_CubeFBOs[3].Model = new Rectangle2D(Math::TVector2I(0.0*size.x,0.75*size.y),Math::TVector2I(0.25*size.x,0.25*size.y));
		m_CubeFBOs[4].Model = new Rectangle2D(Math::TVector2I(0.75*size.x,0.75*size.y),Math::TVector2I(1.0*size.x,0.25*size.y));

		m_CubeFBOs[1].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,-1),Math::TVector2F(1,0),Near, Far);
		m_CubeFBOs[2].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,0),Math::TVector2F(1,1),Near, Far);
		m_CubeFBOs[3].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,-1),Math::TVector2F(0,1),Near, Far);
		m_CubeFBOs[4].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(0,-1),Math::TVector2F(1,1),Near, Far);
		Math::TVector2I NewSize = m_CubeFBOs[0].Fbo->GetSize();
		NewSize.y = NewSize.y / 2;
		for(int i = 1; i < 5; i++)
		{
			m_CubeFBOs[i].Fbo = m_CubeFBOs[0].Fbo->Copy();
			m_CubeFBOs[i].Fbo->SetSize(NewSize);
			if(m_CubeFBOs[i].Model != 0)
				m_CubeFBOs[i].Model->AddTextureMap(DIFFUSE_TEXTURE, m_CubeFBOs[i].Fbo->GetTexture("ColorBuffer"));
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

	void DrawQuad(const Math::TVector2F& BL, const Math::TVector2F& TR)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(BL.x, BL.y);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(BL.x, TR.y);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(TR.x, TR.y);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(TR.x, BL.y);
		glEnd();
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
			Math::CMatrix4 oldProjMatrix = MatrixManager.GetMatrix(PROJECTION_MATRIX);
			for(int i = 0; i < 5; i++)
			{
				m_CubeShader->SetFBO(m_CubeFBOs[i].Fbo, false);
				m_CubeShader->Begin();
				MatrixManager.SetProjectionMatrix(m_CubeFBOs[i].ProjectionMatrix);
				m_CubeShader->SetUniformMatrix4fv("TransformMatrix", m_CubeFBOs[i].TransMatrix);
				RootSceneGraph.Draw();
				m_CubeShader->End();
			}
			MatrixManager.SetProjectionMatrix(oldProjMatrix);
		}

		MatrixManager.SetModeMatrix(MATRIX_2D);
		if(!m_ParaboloidDraw)
		{
			m_2DDraw->Begin();
			for(int i = 0; i < 5; i++)
			{
				if(m_CubeFBOs[i].Model != 0)
					m_CubeFBOs[i].Model->Draw();
			}
			m_2DDraw->End();
		}

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
