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

#include "Application.h"

#include <System/Loaders/Loaders.h>
#include <Addons/Console/DefaultLook.h>

#include <SDLUtilities.h>
namespace ama3D
{


Application* Application::s_Application = NULL;

Application::Application() :
		SettingsManager(CSettingsManager::Instance()), Console(
				CConsole::Instance()), MediaManager(CMediaManager::Instance()), ResourceManager(
				CResourceManager::Instance()), FontManager(
				CFontManager::Instance()), ShaderManager(
				CShaderManager::Instance()), MatrixManager(
				CMatrixManager::Instance()), EventManager(
				CEventManager::Instance()), SceneManager(
				CSceneManager::Instance()), m_IsRunning(true)
{
	s_Application = this;

	// Load all loaders
	Loaders::RegisterAllLoaders();
}

Application::~Application()
{
	// Kill SDL Window
	m_Window.Close();

	// Delete the Scene
	CSceneManager::Destroy();

	// Destroy all manager
	// * Destroy less important Managers
	CConsole::Destroy();
	CMatrixManager::Destroy();
	// * Destroy specials managers (Ressources)
	CFontManager::Destroy();
	CShaderManager::Destroy();
	// * Destroy core managers
	CMediaManager::Destroy();
	CResourceManager::Destroy();
	// * Destroy settings mangers
	CSettingsManager::Destroy();
	// * Destroy the Event manager
	CEventManager::Instance();
	// * Final step: Delete Logger
	//Logger::Kill();
}

void Application::CreateSDLWindow()
{
	// **************************************
	// ********* SDL initialisation *********
	// **************************************
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
//	{
//		std::cout << "[ERROR] On the SDL initialisation : "
//				<< SDL_GetError() << "\n";
//		throw CException("Can't initialize SDL Context.");
//	}
	// OpenGL Version ....
	C3::OpenGLContextSettings settings;
	settings.MajorVersion = 3;
	settings.MinorVersion = 1;
	settings.StentilBits = 8;
	settings.DepthBits = 24;
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// Creation of the SDL Window
	C3::WindowMode settingsWin(
			SettingsManager.GetSizeRenderingWindow().x,
			SettingsManager.GetSizeRenderingWindow().y,false);
	m_Window.Create(settingsWin,"Amaterasu3D", settings);

	// Sync between double buffer and the screen
	//if(SettingsManager.VerticalSync) //<FIXME
	//	m_Window.SetFrameLimit(60);
}

void Application::InitializeOpenGL()
{
	// *******************************
	// ******** OpenGL initialisation
	// *******************************
	glEnable(GL_DEPTH_TEST);

}

void Application::Event(C3::Event& event)
{
	if (event.Type == C3::Event::Closed)
	{
		Exit();
	}
	else if (event.Type  == C3::Event::KeyPressed)
	{
		switch (event.Key.Code)
		{
		case C3::Key::F12:
			Console.Enable(!Console.IsEnable());
			break;
		default:
			break;
		}
	}
	else if(event.Type == C3::Event::TextEntered)
	{
		if (Console.IsEnable())
		{
			char c;
			Console.SendChar(event.Text.Character);
		}
	}
}

void Application::MainLoop()
{
	C3::Event event; ///< To get event from SDL
	while (m_IsRunning)
	{
		// Compute the delta time
		double delta = m_Window.GetFrameTime() / 1000.f;
		//std::cout << delta << std::endl;
		// Manage all events
		while (m_Window.PoolEvent(event))
		{
			Event(event);
			OnEvent(event); ///< To call child method
			if (!Console.IsEnable()) // Console catch all events
				EventManager.OnEvent(event);
		}

		// Update the scene
		OnUpdate(delta);
		EventManager.OnUpdate(delta);

		// Draw the scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		OnRender();

		EventManager.OnEndRender();
		m_Window.Display();
	}
}

void Application::Run()
{
	// Create the Window
	CreateSDLWindow();

	// Check if the system have all requirement
	m_GraphicsDevice.CheckSystem();

	// Initialise the renderer
	InitializeOpenGL();

	// Setup default look
	Console.ChangeLook(new DefaultLook);

	// Event for child class
	OnInitialize();

	// Go to the main loop
	MainLoop();
}

void Application::Exit()
{
	m_IsRunning = false;
}

}
