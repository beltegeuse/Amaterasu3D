//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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
#include "Window.h"
#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <System/Loaders/Loaders.h>
#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif
#include <GL/gl.h>
#include <Debug/OpenGLDebug.h>
#include <System/SettingsManager.h>
#include <Addons/Console/DefaultLook.h>
#include <SDLUtilities.h>

Window::Window(const std::string& name, const Math::TVector2I& windowSize, bool syncVertical) :
	m_isRunning(false),
	m_camera(NULL),
	Console(CConsole::Instance())
{
	CreateWindow(name, windowSize,syncVertical);
}

Window::Window() :
	m_isRunning(false),
	m_camera(NULL),
	Console(CConsole::Instance())
{
	CreateWindow("OpenGL Renderer", SettingsManager::Instance().GetSizeRenderingWindow(),false);
}

Window::~Window()
{
	// Kill SDL Window
	SDL_GL_DeleteContext(m_contexteOpenGL);
	SDL_DestroyWindow(m_fenetre);
	SDL_Quit();
	// Camera delete
	if(m_camera)
		delete m_camera;
	// Delete Managers
	CConsole::Destroy();
}

void Window::CreateWindow(const std::string& name, const Math::TVector2I& windowSize, bool syncVertical)
{
	// **************************************
	// ******* Media initialisation *********
	// **************************************
	Loaders::RegisterAllLoaders(); // TODO: Think to move this
	// **************************************
	// ********* SDL initialisation *********
	// **************************************
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "[ERROR] On the SDL initialisation : " << SDL_GetError() << std::endl;
		SDL_Quit();
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
	m_fenetre = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);
	// Sync between double buffer and the screen
	if(syncVertical)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
	// Enable Unicode support
	// SDL_EnableUNICODE(1); <= Doesn't work
	// **************************************
	// ********* GLEW initialisation ********
	// **************************************
	#ifdef WIN32
	GLenum initialisationGLEW = glewInit();
	// If there is an error
	if(initialisationGLEW != GLEW_OK)
	{
		// Print the error
		std::cout << "[Error] Can't initialize GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;
		// Destroy SDL Window ...
		SDL_GL_DeleteContext(m_contexteOpenGL);
		SDL_DestroyWindow(m_fenetre);
		SDL_Quit();
		throw CException("Can't initialize GLEW.");
	}
	#endif

	// *******************************
	// ******** OpenGL initialisation
	// *******************************
	GLCheck(glEnable(GL_DEPTH_TEST));

	 // Create default console look
	 Console.ChangeLook(new DefaultLook);
}

void Window::Run()
{
	SDL_Event evenements;
	m_isRunning = true;
	int lastTime = SDL_GetTicks();
	// Compteur de FPS
	m_FPS = 0;
	m_timeEslapse = 0.0;
	while(m_isRunning)
	{
		double delta = (SDL_GetTicks() - lastTime) / 1000.0;
		lastTime += delta*1000.0;
		// Events ......
		while(SDL_PollEvent(&evenements)) {
//			std::cout << "Evenet" << std::endl;
			OnEvent(evenements, delta);
		}
		// Mise a jour des FPS
		m_timeEslapse += delta;
		m_FPS += 1;
		if(m_timeEslapse > 1.0)
		{
			std::cout << "[INFO] FPS : " << m_FPS << std::endl;
			m_timeEslapse = m_timeEslapse - 1.0;
			m_FPS = 0;
		}
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    // Draw all the Scene ...
		if(m_camera)
			m_camera->ComputeMatrix(delta);
	    OnDraw(delta);
	    // Swap buffers
	    SDL_GL_SwapWindow(m_fenetre);
	    // Console Update
	    Console.Update();
	}
}

void Window::OnEvent(SDL_Event& events, double delta)
{
	if(events.window.event == SDL_WINDOWEVENT_CLOSE)
		m_isRunning = false;
	if(events.type == SDL_KEYDOWN)
	{
//		std::cout << "Key Down !" << std::endl;
		if(events.key.keysym.sym == SDLK_F12)
		{
			Console.Enable(!Console.IsEnable());
		}
		char c;
		if(GetSDLChar(events, &c) && Console.IsEnable())
		{
//			std::cout << c << "\n";
			Console.SendChar(c);
			return;
		}
	}
	if(m_camera)
		m_camera->OnEvent(events, delta);
}

void Window::OnDraw(double DeltaTime)
{
	////////////////////////////////////////////////
	// G-Buffer stage
	////////////////////////////////////////////////

	if(m_camera)
	{
		m_camera->GetView();
	}
	// Do all graphics part here
	// Draw the SceneGraph
	m_root.Draw();

}

SceneGraph::Group& Window::GetSceneRoot()
{
	return m_root;
}

void Window::SetCamera(CameraAbstract* camera)
{
	if(m_camera)
		delete m_camera;
	m_camera = camera;
}
