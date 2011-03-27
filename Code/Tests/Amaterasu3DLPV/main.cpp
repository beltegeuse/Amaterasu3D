#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Model.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Logger/LoggerFile.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Application.h>
#include <Graphics/Lighting/LightingStructures.h>
#include <Addons/FPS/FPS.h>
class ApplicationLPV : public Application
{
protected:
	// Shaders
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	// Camera
	CameraFPS* m_Camera;
	// FPS Counter
	FPS m_FPS;
	// Light
	SpotLight m_Light;
	// Debug
	bool m_Debug;
	bool m_DebugGBuffer;
	bool m_DebugCompositing;
public:

	std::string ShowInfoCamera()
	{
		std::stringstream ss;
		ss << "Pos : " << m_Camera->GetPosition() << "\n";
		ss << "Target : " << m_Camera->GetTarget();
		return ss.str();
	}

	void UpdateLightPosition()
	{
		m_Light.Position = m_Camera->GetPosition();
		m_Light.Direction = m_Camera->GetTarget();
		m_Light.Direction.Normalize();
	}

	virtual ~ApplicationLPV()
	{
	}

private:
	/*
	 * Virtual functions
	 */
	//! Update all scene objects
	virtual void OnUpdate(double deltaTime)
	{
	}

	virtual void OnEvent(SDL_Event& event){
		if(event.type == SDL_KEYDOWN)
		{
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_Debug = !m_Debug;
					 break;
				 case SDLK_F2:
					 m_DebugGBuffer = !m_DebugGBuffer;
					 break;
				 case SDLK_F3:
					 m_DebugCompositing = !m_DebugCompositing;
					 break;
			 }
		}
	}
	//! Make all initializations
	virtual void OnInitialize()
	{
		// Setup variables
		m_Debug = false;
		m_DebugGBuffer = false;
		m_DebugCompositing = false;
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(6,102,72), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Initialise OpenGL
		glClearColor(0.0f,0.0f,0.0f,1.f);
		MatrixManager.SetProjectionMatrix(Math::CMatrix4::CreatePerspectiveFOV(70, (double)800/600, 1.0, 400));
		// Load shader
		m_GBufferShader = ShaderManager.LoadShader("GBuffer.shader");
//		m_RSMSpotShader = ShaderManager.LoadShader("RefectiveShadowMapSpot.shader");
		// Create light
		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_Light.Position = Math::TVector3F(59.452,123.893,61.3002);
		m_Light.LightRaduis = 100.0;
		m_Light.LightIntensity = 1.0;
		m_Light.LightCutOff = 300;
		m_Light.Direction = Math::TVector3F(-58.5615,-123.439,-61.2961);
		m_Light.Direction.Normalize();
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);
		// Console commands
		Console.RegisterCommand("camera",Console::Bind(&ApplicationLPV::ShowInfoCamera, *this));
		Console.RegisterCommand("updatelight",Console::Bind(&ApplicationLPV::UpdateLightPosition, *this));
	}

	//! Draw the scene
	virtual void OnRender()
	{
		// =========== First STEPS (GBuffer generation)
		// Fill in the GBuffer
		m_GBufferShader->begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_GBufferShader->end();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		// ========== Second STEPS (RSM generation buffers)
//		// Fill in RSM spot buffers
//		// * Matrix Setup
//		Math::CMatrix4 LightViewMatrix;
//		LightViewMatrix.LookAt(m_Light.Position, m_Light.Direction);
//		Math::CMatrix4 LightProjectionMatrix;
//		LightProjectionMatrix.PerspectiveFOV(m_Light.LightCutOff, 512.0/512.0, 1.0, m_Light.LightRaduis);
//		Math::CMatrix4 oldProjectionMatrix;
//		Math::CMatrix4 oldViewMatrix;
//		// * Save old transformations
//		oldProjectionMatrix = MatrixManager.GetMatrix(PROJECTION_MATRIX);
//		oldViewMatrix = MatrixManager.GetMatrix(VIEW_MATRIX);
//		// * Go to the camera view
//		MatrixManager.SetProjectionMatrix(LightProjectionMatrix);
//		MatrixManager.SetViewMatrix(LightViewMatrix);
//		// * Enable Shader
//		m_RSMSpotShader->begin();
//		// *** Send all Uniform values
//		m_RSMSpotShader->setUniform1f("LightRaduis",m_Light.LightRaduis);
//		m_RSMSpotShader->setUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
//		m_RSMSpotShader->setUniform1f("LightIntensity", m_Light.LightIntensity);
//		m_RSMSpotShader->setUniform3f("LightPosition", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
//		m_RSMSpotShader->setUniform3f("LightSpotDirection", m_Light.Direction.x, m_Light.Direction.y, m_Light.Direction.z);
//		m_RSMSpotShader->setUniform3f("LightColor", m_Light.LightColor.R, m_Light.LightColor.G, m_Light.LightColor.B);
//		// * Draw the scene
//		RootSceneGraph.Draw();
//		m_RSMSpotShader->end();
//		// * Revert transformations
//		MatrixManager.SetProjectionMatrix(oldProjectionMatrix);
//		MatrixManager.SetViewMatrix(oldViewMatrix);

		m_GBufferShader->GetFBO()->DrawDebug();

//		if(m_DebugGBuffer)
//		{
//			m_RSMSpotShader->GetFBO()->DrawDebug();
//		}

		Console.Draw();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	// FIXME: Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;
	ApplicationLPV application;
	application.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
