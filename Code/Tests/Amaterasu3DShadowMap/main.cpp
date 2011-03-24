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
#include <Graphics/MatrixManagement.h>
#include <System/SettingsManager.h>
#include <Graphics/Font/FontManager.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class WindowShadow : public Window
{
protected:
	TShaderPtr m_BasicShaderShadow;
	TShaderPtr m_BasicShader;
	TShaderPtr m_ShadowShader;
	Math::CMatrix4 m_matrixPerspective;
	SpotLight m_light;

	bool m_debug;
	bool m_showDepth;
	bool m_cameraView;
public:
	WindowShadow() :
		Window("WindowShadow"),
		m_debug(false),
		m_showDepth(false),
		m_cameraView(false)
	{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		cam->SetSpeed(20.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 0.1, 100);
		CMatrixManager::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Shader loading
		m_BasicShaderShadow = CShaderManager::Instance().LoadShader("DebugDrawShadowMapOnly.shader");
		m_BasicShader = CShaderManager::Instance().LoadShader("BasicShader.shader");
		m_ShadowShader = CShaderManager::Instance().LoadShader("ShadowMap.shader");

		// Create Light
		m_light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_light.Position = Math::TVector3F(7,10,7);
		m_light.LightRaduis = 40.0;
		m_light.LightIntensity = 1.0;
		m_light.LightCutOff = 70;
		m_light.Direction = Math::TVector3F(4.0,4.0,4.0);
		// Load scene
		// * Create first cube
		SceneGraph::Group * cubeGroup = new SceneGraph::Group;
		cubeGroup->AddChild(new DebugCubeLeaf);
		Math::CMatrix4 matCube;
		matCube.SetTranslation(4,4,4);
		cubeGroup->LoadTransformMatrix(matCube);
		// * Create floor
		SceneGraph::Group * cubeFloor = new SceneGraph::Group;
		cubeFloor->AddChild(new DebugCubeLeaf);
		Math::CMatrix4 matFloor;
		matFloor.SetTranslation(0,0,0);
		matFloor.SetScaling(10,1,10);
		cubeFloor->LoadTransformMatrix(matFloor);
		// Add to root
		GetSceneRoot().AddChild(cubeGroup);
		GetSceneRoot().AddChild(cubeFloor);
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
					 m_showDepth = !m_showDepth;
				 case SDLK_F3:
					 m_cameraView = !m_cameraView;
			 }
		}

	}

	virtual void OnDraw(double delta)
	{
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_light.Position, m_light.Direction);
		Math::CMatrix4 LightProjectionMatrix;
		Math::CMatrix4 oldProjectionMatrix;
		Math::CMatrix4 oldViewMatrix;
//		LightProjectionMatrix.PerspectiveFOV(m_light.LightCutOff, 512.0/512.0, 1.0, m_light.LightRaduis);
//		{
//			// Generate the Shadow Map
//			// * Transformations
//			oldProjectionMatrix = MatrixManagement::Instance().GetMatrix(PROJECTION_MATRIX);
//			oldViewMatrix = MatrixManagement::Instance().GetMatrix(VIEW_MATRIX);
//			MatrixManagement::Instance().SetProjectionMatrix(LightProjectionMatrix);
//			MatrixManagement::Instance().SetViewMatrix(LightViewMatrix);
//			// * Draw the scene
//			glEnable(GL_CULL_FACE);
//			glCullFace(GL_BACK);
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//			m_ShadowShader->begin();
//			GetSceneRoot().Draw(); // Draw the scene
//			m_ShadowShader->end();
//			glDisable(GL_CULL_FACE);
//			// * Revert transformations
//			MatrixManagement::Instance().SetProjectionMatrix(oldProjectionMatrix);
//			MatrixManagement::Instance().SetViewMatrix(oldViewMatrix);
//		}
//
//		m_ShadowShader->GetFBO()->DrawDebug();

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


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw Tow 2D things
		Console.Draw();
//
//		glMatrixMode(GL_PROJECTION);
//		glPushMatrix();
//		glLoadIdentity();
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//		glLoadIdentity();
//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	// FIXME: Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowShadow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
