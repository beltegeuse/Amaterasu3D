//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================
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
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>
#include <Addons/Logo/Logo.h>

class ApplicationShadow : public Application
{
protected:
	FPS m_FPS;
	Logo m_Logo;
	CameraFPS* m_Camera;
	TShaderPtr m_BasicShaderShadow;
	TShaderPtr m_BasicShader;
	TShaderPtr m_ShadowShader;
//	TShaderPtr m_GBufferShader;
	Math::CMatrix4 m_matrixPerspective;
	SpotLight m_Light;

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

	void UpdateLightPosition()
	{
		m_Light.Position = m_Camera->GetPosition();
		m_Light.Direction = m_Camera->GetTarget();
	}

	virtual void OnInitialize()
	{
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(20.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective = m_matrixPerspective.CreatePerspectiveFOV(70, (double)800/600, 1.0, 400);
		CMatrixManager::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Shader loading
		m_BasicShaderShadow = CShaderManager::Instance().LoadShader("DebugDrawShadowMapOnly.shader");
		// m_BasicShader = CShaderManager::Instance().LoadShader("BasicShader.shader");
		m_ShadowShader = CShaderManager::Instance().LoadShader("ShadowMap.shader");
//		m_GBufferShader = CShaderManager::Instance().LoadShader("GBuffer.shader");

		// Create Light
		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_Light.Position = Math::TVector3F(7,10,7);
		m_Light.LightRaduis = 40.0;
		m_Light.LightIntensity = 1.0;
		m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(4.0,4.0,4.0);
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
		RootSceneGraph.AddChild(cubeGroup);
		RootSceneGraph.AddChild(cubeFloor);

		Console.RegisterCommand("updatelight",Console::Bind(&ApplicationShadow::UpdateLightPosition, *this));
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
		MatrixManager.SetModeMatrix(MATRIX_3D);
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_Light.Position, m_Light.Direction);
		Math::CMatrix4 LightProjectionMatrix;
		Math::CMatrix4 oldProjectionMatrix;
		Math::CMatrix4 oldViewMatrix;
		LightProjectionMatrix = LightProjectionMatrix.CreatePerspectiveFOV(m_Light.LightCutOff, (double)512.0/512.0, 1.0, m_Light.LightRaduis);
		{
			// Generate the Shadow Map
			// * Transformations
			oldProjectionMatrix = MatrixManager.GetMatrix(PROJECTION_MATRIX);
			oldViewMatrix = MatrixManager.GetMatrix(VIEW_MATRIX);
			MatrixManager.SetProjectionMatrix(LightProjectionMatrix);
			MatrixManager.SetViewMatrix(LightViewMatrix);
			// * Draw the scene
			// glEnable(GL_CULL_FACE);
			// glCullFace(GL_BACK);
			// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_ShadowShader->Begin();
			RootSceneGraph.Draw(); // Draw the scene
			m_ShadowShader->End();
			// glDisable(GL_CULL_FACE);
			// * Revert transformations
			MatrixManager.SetProjectionMatrix(oldProjectionMatrix);
			MatrixManager.SetViewMatrix(oldViewMatrix);
		}
		m_ShadowShader->GetFBO()->DrawDebug();

		{
			if(m_cameraView)
			{
				oldProjectionMatrix = MatrixManager.GetMatrix(PROJECTION_MATRIX);
				oldViewMatrix = MatrixManager.GetMatrix(VIEW_MATRIX);
				MatrixManager.SetProjectionMatrix(LightProjectionMatrix);
				MatrixManager.SetViewMatrix(LightViewMatrix);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_ShadowShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+0);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			m_BasicShaderShadow->Begin();
			m_BasicShaderShadow->SetUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
			m_BasicShaderShadow->SetUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
			m_BasicShaderShadow->SetUniform1i("DebugMode", m_debug);
			if(!m_cameraView)
			{
				m_Camera->GetView();
			}
			RootSceneGraph.Draw();
			m_BasicShaderShadow->End();
			m_ShadowShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			if(m_cameraView)
			{
				MatrixManager.SetProjectionMatrix(oldProjectionMatrix);
				MatrixManager.SetViewMatrix(oldViewMatrix);
			}
			glDisable(GL_CULL_FACE);
		}


		// Draw Tow 2D things
		MatrixManager.SetModeMatrix(MATRIX_2D);
		m_Logo.Draw();
		Console.Draw();
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
