#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
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

class ApplicationLPV : public Application
{
protected:
	// Shaders
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	// Camera
	CameraFPS* m_Camera;
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
		//ss << "Camera : " << *m_camera;
		return ss.str();
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
		CMatrixManager::Instance().SetProjectionMatrix(Math::CMatrix4::PerspectiveFOV(70, (double)800/600, 1.0, 400));
		// Load shader
		m_GBufferShader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		m_RSMSpotShader = CShaderManager::Instance().LoadShader("RefectiveShadowMapSpot.shader");
		// Create light
		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_Light.Position = Math::TVector3F(0,10.0/3.0,6.0);
		m_Light.LightRaduis = 100.0;
		m_Light.LightIntensity = 1.0;
		m_Light.LightCutOff = 300;
		m_Light.Direction = Math::TVector3F(0.0,-0.6,-1.4);
		m_Light.Direction.Normalize();
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);

		Console.RegisterCommand("camera",Console::Bind(&ApplicationLPV::ShowInfoCamera, *this));
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

//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		// ========== Second STEPS (RSM generation buffers)
//		// Fill in RSM spot buffers
//		// * Matrix Setup
//		Math::CMatrix4 LightViewMatrix;
//		LightViewMatrix.LookAt(m_light.Position, m_light.Direction);
//		Math::CMatrix4 LightProjectionMatrix;
//		LightProjectionMatrix.PerspectiveFOV(m_light.LightCutOff, 800.0/600.0, 1.0, m_light.LightRaduis);
//		Math::CMatrix4 oldProjectionMatrix;
//		Math::CMatrix4 oldViewMatrix;
//		// * Save old transformations
//		oldProjectionMatrix = CMatrixManager::Instance().GetMatrix(PROJECTION_MATRIX);
//		oldViewMatrix = CMatrixManager::Instance().GetMatrix(VIEW_MATRIX);
//		// * Go to the camera view
//		CMatrixManager::Instance().SetProjectionMatrix(LightProjectionMatrix);
//		CMatrixManager::Instance().SetViewMatrix(LightViewMatrix);
//		// * Enable Shader
//		m_RSMSpotShader->begin();
//		// *** Send all Uniform values
//		m_RSMSpotShader->setUniform1f("LightRaduis",m_light.LightRaduis);
//		m_RSMSpotShader->setUniform1f("LightCutOff", cos(m_light.LightCutOff *(M_PI / 180.0)));
//		m_RSMSpotShader->setUniform1f("LightIntensity", m_light.LightIntensity);
//		m_RSMSpotShader->setUniform3f("LightPosition", m_light.Position.x, m_light.Position.y, m_light.Position.z);
//		m_RSMSpotShader->setUniform3f("LightSpotDirection", m_light.Direction.x, m_light.Direction.y, m_light.Direction.z);
//		m_RSMSpotShader->setUniform3f("LightColor", m_light.LightColor.R, m_light.LightColor.G, m_light.LightColor.B);
//		// * Draw the scene
//		GetSceneRoot().Draw();
//		m_RSMSpotShader->end();
//		// * Revert transformations
//		CMatrixManager::Instance().SetProjectionMatrix(oldProjectionMatrix);
//		CMatrixManager::Instance().SetViewMatrix(oldViewMatrix);
//
		m_GBufferShader->GetFBO()->DrawDebug();

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
