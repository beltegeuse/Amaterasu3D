#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/Camera/CameraFly.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <System/SettingsManager.h>
#include <Graphics/Font/FontManager.h>
#include <Graphics/MatrixManagement.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <boost/random.hpp>

class WindowLPV : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	Texture * m_textureRand;
	bool m_debug;
	bool m_debugGBuffer;
	bool m_debugCompositing;
	SpotLight m_light;
public:
	WindowLPV() :
		Window("Amaterasu3DTestApp"),
		m_debug(false),
		m_debugGBuffer(false),
		m_debugCompositing(false)
	{
		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(6,102,72), Math::TVector3F(0,0,0));
		cam->SetSpeed(100.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1.0, 400);
		MatrixManagement::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Load shader
		m_GBufferShader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		m_RSMSpotShader = glShaderManager::Instance().LoadShader("RefectiveShadowMapSpot.shader");
		// Create light
		m_light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_light.Position = Math::TVector3F(0,10.0/3.0,6.0);
		m_light.LightRaduis = 100.0;
		m_light.LightIntensity = 1.0;
		m_light.LightCutOff = 300;
		m_light.Direction = Math::TVector3F(0.0,-0.6,-1.4);
		m_light.Direction.Normalize();
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		GetSceneRoot().AddChild(node);

		Console.RegisterCommand("camera",Console::Bind(&WindowLPV::ShowInfoCamera, *this));
	}

	std::string ShowInfoCamera()
	{
		std::stringstream ss;
		ss << "Camera : " << *m_camera;
		return ss.str();
	}

	virtual ~WindowLPV()
	{
	}

	virtual void OnEvent(SDL_Event& event, double delta)
	{
		Window::OnEvent(event, delta);
		if(event.type == SDL_KEYDOWN)
		{
			Math::CMatrix4 matrixTransform;
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_debug = !m_debug;
					 break;
				 case SDLK_F2:
					 m_debugGBuffer = !m_debugGBuffer;
					 break;
				 case SDLK_F3:
					 m_debugCompositing = !m_debugCompositing;
					 break;
			 }
		}

	}

	virtual void OnDraw(double delta)
	{
		// =========== First STEPS (GBuffer generation)
		// Fill in the GBuffer
		m_GBufferShader->begin();
		Window::OnDraw(delta);
		m_GBufferShader->end();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ========== Second STEPS (RSM generation buffers)
		// Fill in RSM spot buffers
		// * Matrix Setup
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_light.Position, m_light.Direction);
		Math::CMatrix4 LightProjectionMatrix;
		LightProjectionMatrix.PerspectiveFOV(m_light.LightCutOff, 800.0/600.0, 1.0, m_light.LightRaduis);
		Math::CMatrix4 oldProjectionMatrix;
		Math::CMatrix4 oldViewMatrix;
		// * Save old transformations
		oldProjectionMatrix = MatrixManagement::Instance().GetMatrix(PROJECTION_MATRIX);
		oldViewMatrix = MatrixManagement::Instance().GetMatrix(VIEW_MATRIX);
		// * Go to the camera view
		MatrixManagement::Instance().SetProjectionMatrix(LightProjectionMatrix);
		MatrixManagement::Instance().SetViewMatrix(LightViewMatrix);
		// * Enable Shader
		m_RSMSpotShader->begin();
		// *** Send all Uniform values
		m_RSMSpotShader->setUniform1f("LightRaduis",m_light.LightRaduis);
		m_RSMSpotShader->setUniform1f("LightCutOff", cos(m_light.LightCutOff *(M_PI / 180.0)));
		m_RSMSpotShader->setUniform1f("LightIntensity", m_light.LightIntensity);
		m_RSMSpotShader->setUniform3f("LightPosition", m_light.Position.x, m_light.Position.y, m_light.Position.z);
		m_RSMSpotShader->setUniform3f("LightSpotDirection", m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);
		m_RSMSpotShader->setUniform3f("LightColor", m_light.LightColor.R, m_light.LightColor.G, m_light.LightColor.B);
		// * Draw the scene
		GetSceneRoot().Draw();
		m_RSMSpotShader->end();
		// * Revert transformations
		MatrixManagement::Instance().SetProjectionMatrix(oldProjectionMatrix);
		MatrixManagement::Instance().SetViewMatrix(oldViewMatrix);

		m_GBufferShader->GetFBO()->DrawDebug();

		Console.Draw();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	// FIXME: Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowLPV window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
