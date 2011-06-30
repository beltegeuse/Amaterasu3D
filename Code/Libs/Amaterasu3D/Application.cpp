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

Application* Application::s_Application = NULL;

Application::Application() :
	SettingsManager(CSettingsManager::Instance()),
	Console(CConsole::Instance()),
	MediaManager(CMediaManager::Instance()),
	ResourceManager(CResourceManager::Instance()),
	FontManager(CFontManager::Instance()),
	ShaderManager(CShaderManager::Instance()),
	MatrixManager(CMatrixManager::Instance()),
	EventManager(CEventManager::Instance()),
	SceneManager(CSceneManager::Instance()),
	m_TimeElapse(0.0),
	m_IsRunning(true)
{
	s_Application = this;

	// Load all loaders
	Loaders::RegisterAllLoaders();
}

Application::~Application()
{
	// Kill SDL Window
	SDL_GL_DeleteContext(m_SDLOpenGLContext);
	SDL_DestroyWindow(m_SDLFenetre);
	SDL_Quit();

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
	Logger::Kill();
}

void Application::CreateSDLWindow()
{
	// **************************************
	// ********* SDL initialisation *********
	// **************************************
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Logger::Log() << "[ERROR] On the SDL initialisation : " << SDL_GetError() << "\n";
		throw CException("Can't initialize SDL Context.");
	}
	// OpenGL Version ....
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	// OpenGL Buffer instructions
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// Creation of the SDL Window
	m_SDLFenetre = SDL_CreateWindow("Amaterasu3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			    SettingsManager.GetSizeRenderingWindow().x,
			    SettingsManager.GetSizeRenderingWindow().y,
			    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	m_SDLOpenGLContext = SDL_GL_CreateContext(m_SDLFenetre);
	// Sync between double buffer and the screen
	if(SettingsManager.VerticalSync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
}

void Application::InitializeOpenGL()
{
	// *******************************
	// ******** OpenGL initialisation
	// *******************************
	glEnable(GL_DEPTH_TEST);

}

void Application::Event(SDL_Event& event)
{
	if(event.window.event == SDL_WINDOWEVENT_CLOSE)
	{
		Exit();
	}
	if(event.type == SDL_KEYDOWN)
	{
		 switch(event.key.keysym.sym)
		 {
			 case SDLK_F12:
				 Console.Enable(!Console.IsEnable());
				 break;
			 default:
				 break;
		 }

		 if(Console.IsEnable())
		 {
			 char c;
			 if(GetSDLChar(event, &c))
				 Console.SendChar(c);
		 }
	}
}

void Application::MainLoop()
{
	SDL_Event event; ///< To get event from SDL
	m_TimeElapse = SDL_GetTicks(); // Get the time from SDL
	while (m_IsRunning)
	 {
		 // Compute the delta time
		 double delta = (SDL_GetTicks() - m_TimeElapse) / 1000.0;
		 m_TimeElapse += delta*1000.0;

		 // Manage all events
		 while(SDL_PollEvent(&event))
		 {
			 Event(event);
			 OnEvent(event); ///< To call child method
			 if(!Console.IsEnable()) // Console catch all events
				 EventManager.OnEvent(event);
		 }

		 // Update the scene
		 OnUpdate(delta);
		 EventManager.OnUpdate(delta);

		 // Draw the scene
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 OnRender();

		 EventManager.OnEndRender();
		 SDL_GL_SwapWindow(m_SDLFenetre);
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



