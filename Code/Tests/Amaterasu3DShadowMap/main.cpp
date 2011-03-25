#include <iostream>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>

class ApplicationShadow : public Application
{
protected:
	FPS m_FPS;
	CameraFPS* m_Camera;
	TShaderPtr m_BasicShaderShadow;
	TShaderPtr m_BasicShader;
	TShaderPtr m_ShadowShader;
	TShaderPtr m_GBufferShader;
	Math::CMatrix4 m_matrixPerspective;
	SpotLight m_light;

	bool m_debug;
	bool m_showDepth;
	bool m_cameraView;
public:
	ApplicationShadow() :
		m_debug(false),
		m_showDepth(false),
		m_cameraView(false)
	{
	}

	virtual void OnInitialize()
	{
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Camera Setup
//		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera = new CameraFPS(Math::TVector3F(6,102,72), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(2.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1.0, 400);
		CMatrixManager::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Shader loading
//		m_BasicShaderShadow = CShaderManager::Instance().LoadShader("DebugDrawShadowMapOnly.shader");
//		m_BasicShader = CShaderManager::Instance().LoadShader("BasicShader.shader");
//		m_ShadowShader = CShaderManager::Instance().LoadShader("ShadowMap.shader");
		m_GBufferShader = CShaderManager::Instance().LoadShader("GBuffer.shader");

		// Create Light
		m_light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_light.Position = Math::TVector3F(7,10,7);
		m_light.LightRaduis = 40.0;
		m_light.LightIntensity = 1.0;
		m_light.LightCutOff = 70;
		m_light.Direction = Math::TVector3F(4.0,4.0,4.0);
		// Load scene
		// * Create first cube
//		SceneGraph::Group * cubeGroup = new SceneGraph::Group;
//		cubeGroup->AddChild(new DebugCubeLeaf);
//		Math::CMatrix4 matCube;
//		matCube.SetTranslation(4,4,4);
//		cubeGroup->LoadTransformMatrix(matCube);
//		// * Create floor
//		SceneGraph::Group * cubeFloor = new SceneGraph::Group;
//		cubeFloor->AddChild(new DebugCubeLeaf);
//		Math::CMatrix4 matFloor;
//		matFloor.SetTranslation(0,0,0);
//		matFloor.SetScaling(10,1,10);
//		cubeFloor->LoadTransformMatrix(matFloor);
//		// Add to root
//		RootSceneGraph.AddChild(cubeGroup);
//		RootSceneGraph.AddChild(cubeFloor);
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);
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
				m_showDepth = !m_showDepth;
			case SDLK_F3:
				m_cameraView = !m_cameraView;
			}
		}

	}

	virtual void OnUpdate(double delta)
	{}

	virtual void OnRender()
	{
		glEnable(GL_DEPTH_TEST);
//		std::cout << "Render..." << std::endl;
//		Math::CMatrix4 LightViewMatrix;
//		LightViewMatrix.LookAt(m_light.Position, m_light.Direction);
//		Math::CMatrix4 LightProjectionMatrix;
//		Math::CMatrix4 oldProjectionMatrix;
//		Math::CMatrix4 oldViewMatrix;
//		LightProjectionMatrix.PerspectiveFOV(m_light.LightCutOff, (double)800/600, 1.0, m_light.LightRaduis);
//		{
//			// Generate the Shadow Map
//			// * Transformations
//			oldProjectionMatrix = MatrixManager.GetMatrix(PROJECTION_MATRIX);
//			oldViewMatrix = MatrixManager.GetMatrix(VIEW_MATRIX);
//			MatrixManager.SetProjectionMatrix(LightProjectionMatrix);
//			MatrixManager.SetViewMatrix(LightViewMatrix);
//			// * Draw the scene
////			glEnable(GL_CULL_FACE);
////			glCullFace(GL_BACK);
////			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//			m_ShadowShader->begin();
//			RootSceneGraph.Draw(); // Draw the scene
//			m_ShadowShader->end();
////			glDisable(GL_CULL_FACE);
//			// * Revert transformations
//			MatrixManager.SetProjectionMatrix(oldProjectionMatrix);
//			MatrixManager.SetViewMatrix(oldViewMatrix);
//		}
//		std::cout << "Show shadow..." << std::endl;
//		m_ShadowShader->GetFBO()->DrawDebug();

		m_GBufferShader->begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_GBufferShader->end();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_GBufferShader->GetFBO()->DrawDebug();
		//		if(true)
		//		{
		//
		//		}

		//		{
		//			if(m_cameraView)
		//			{
		//				oldProjectionMatrix = MatrixManagement::Instance().GetMatrix(PROJECTION_MATRIX);
		//				oldViewMatrix = MatrixManagement::Instance().GetMatrix(VIEW_MATRIX);
		//				MatrixManagement::Instance().SetProjectionMatrix(LightProjectionMatrix);
		//				MatrixManagement::Instance().SetViewMatrix(LightViewMatrix);
		//			}
		//
		//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//			m_ShadowShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+0);
		//			glEnable(GL_CULL_FACE);
		//			glCullFace(GL_BACK);
		//			m_BasicShaderShadow->begin();
		//			m_BasicShaderShadow->setUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
		//			m_BasicShaderShadow->setUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
		//			m_BasicShaderShadow->setUniform1i("DebugMode", m_debug);
		//			if(!m_cameraView)
		//			{
		//				m_camera->GetView();
		//			}
		//			GetSceneRoot().Draw();
		//			m_BasicShaderShadow->end();
		//			m_ShadowShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		//			if(m_cameraView)
		//			{
		//				MatrixManagement::Instance().SetProjectionMatrix(oldProjectionMatrix);
		//				MatrixManagement::Instance().SetViewMatrix(oldViewMatrix);
		//			}
		//			glDisable(GL_CULL_FACE);
		//		}


		// Draw Tow 2D things
		//		Console.Draw();
		//
		//		glMatrixMode(GL_PROJECTION);
		//		glPushMatrix();
		//		glLoadIdentity();
		//		glMatrixMode(GL_MODELVIEW);
		//		glPushMatrix();
		//		glLoadIdentity();
		//
		//
		//		glPopMatrix();
		//		glMatrixMode(GL_PROJECTION);
		//		glPopMatrix();
		//		glMatrixMode(GL_MODELVIEW);

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;
	ApplicationShadow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
