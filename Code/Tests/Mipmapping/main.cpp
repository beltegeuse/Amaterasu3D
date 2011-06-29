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
#include <Addons/ToneMappingOperator/ManualMipmapping.h>

class ApplicationMipMapping : public Application
{
protected:
	CameraFPS* m_Camera;
	CameraAnimation* m_CameraAnimation;
	TShaderPtr m_gbuffer_shader;
	TShaderPtr m_HardwareMipmapping;
	TShaderPtr m_ShowMipmapping;
	DeferredLighting* m_GI;
	ManualMipmapping m_ManualMipmapping;
	FPS m_FPS;
	bool m_debug;
	bool m_play;
	bool m_autoMapping;
	int m_level;
public:
	/*
	 * Console methods
	 */

	void ConsoleRead(const std::string& path)
	{
		std::string fullpath = path+".xml";
		m_CameraAnimation->ReadXMLFile(fullpath);
	}

	/*
	 * Constructor and destructors
	 */
	ApplicationMipMapping() :
		m_debug(false),
		m_play(false),
		m_autoMapping(false),
		m_ManualMipmapping(128),
		m_level(0)
	{

	}

	virtual ~ApplicationMipMapping()
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
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(1.0,4000.0,70.0);
		// Load shader
		m_gbuffer_shader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		m_HardwareMipmapping = CShaderManager::Instance().LoadShader("CreateMipmapping.shader");
		m_ShowMipmapping = CShaderManager::Instance().LoadShader("TestMipmappingRead.shader");
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
		IMeshSceneNode* scene = SceneManager.LoadMesh("sponza.obj",0);
		SceneManager.AddScenegraphRoot(scene);

		m_ManualMipmapping.Initialize();

		// Add Console functions
		Console.RegisterCommand("read",Console::Bind(&ApplicationMipMapping::ConsoleRead, *this));
	}

	virtual void OnUpdate(double delta)
	{
		if(m_play)
			m_CameraAnimation->Update(delta);
	}

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
					 m_GI->SetDebugMode(!m_GI->isDebugMode());
					 break;
				 case SDLK_F3:
					 m_level++;
					 m_level = m_level % (m_ManualMipmapping.NumberLevels());
					 break;
				 case SDLK_F4:
					 m_autoMapping = !m_autoMapping;
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

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if(m_autoMapping)
		{
			m_HardwareMipmapping->Begin();
			m_gbuffer_shader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
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
			m_gbuffer_shader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_HardwareMipmapping->End();

			m_ShowMipmapping->Begin();
			m_HardwareMipmapping->GetFBO()->GetTexture("ColorBuffer")->activateMultiTex(CUSTOM_TEXTURE);
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
			m_HardwareMipmapping->GetFBO()->GetTexture("ColorBuffer")->desactivateMultiTex(CUSTOM_TEXTURE);
			m_ShowMipmapping->End();
		}
		else
		{
			m_ManualMipmapping.Compute(m_gbuffer_shader->GetFBO()->GetTexture("Diffuse"));

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			m_ManualMipmapping.GetLevel(m_level)->activateTextureMapping();
			m_ManualMipmapping.GetLevel(m_level)->activateTexture();

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
			m_ManualMipmapping.GetLevel(m_level)->desactivateTextureMapping();
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

	ApplicationMipMapping window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
