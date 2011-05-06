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
	TShaderPtr m_CubeShader;
	TShaderPtr m_volumeRenderingShader;
	TTexturePtr m_VolumeTexture;
	FBO * m_FrontFBO;
	FBO * m_BackFBO;
	BinvoxModel* m_BinVox;
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
		m_Camera = new CameraFPS(Math::TVector3F(30,40,20), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Shaders
		m_CubeShader = ShaderManager.LoadShader("CubePass.shader");
		m_volumeRenderingShader = ShaderManager.LoadShader("VolumeRendering.shader");
		// FBO
		m_BackFBO = m_CubeShader->GetFBO()->Copy();
		m_FrontFBO = m_CubeShader->GetFBO();
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(0.1,1000.0,70.0);
		// Load scene
		Logger::Log() << "[INFO] Load Armadillo.binvox\n";
		m_BinVox = new BinvoxModel("Dragon.binvox");
		m_VolumeTexture = m_BinVox->Create2DTexture();
		//RootSceneGraph.AddChild(m_BinVox->CreateDebugPointModel());
		RootSceneGraph.AddChild(m_BinVox->CreateCoordinateCubeModel());
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

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		m_CubeShader->SetFBO(m_FrontFBO, false);
		m_CubeShader->Begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_CubeShader->End();

//		m_CubeShader->GetFBO()->DrawDebug();

		glCullFace(GL_FRONT);
		m_CubeShader->SetFBO(m_BackFBO, false);
		m_CubeShader->Begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_CubeShader->End();
		glDisable(GL_CULL_FACE);

		Math::TVector2I repeatTex = m_BinVox->TextureRepeat();
		Math::TVector2I sizeTex = m_BinVox->TextureSize();

		m_volumeRenderingShader->Begin();
		m_FrontFBO->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_BackFBO->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_VolumeTexture->activateMultiTex(CUSTOM_TEXTURE+2);
		m_volumeRenderingShader->SetUniformVector("GridDimension", m_BinVox->GridSize());
		m_volumeRenderingShader->SetUniformVector("GridTextureSize", Math::TVector4F(sizeTex.x, sizeTex.y, repeatTex.x, repeatTex.y));
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-1.0, -1.0);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-1.0, 1.0);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_volumeRenderingShader->End();

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
