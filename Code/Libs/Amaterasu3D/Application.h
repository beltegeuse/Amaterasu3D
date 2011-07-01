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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <SDL/SDL.h>
#include <System/MediaManager.h>
#include <System/ResourceManager.h>
#include <System/SettingsManager.h>
#include <Addons/Console/Console.h>
#include <Graphics/Font/FontManager.h>
#include <Graphics/Shaders/Shader.h>
#include <Graphics/MatrixManagement.h>
#include <Graphics/SceneManager.h>
#include <System/EventManager.h>
#include <System/GraphicsDevice.h>

namespace ama3D
{

/////////////////////////////////
/// Derivate this class to create an application
/////////////////////////////////
class Application
{
public:
	//! Launch the application
	void Run();

	// Managers
	CSettingsManager& SettingsManager;
	CConsole& Console;
	CMediaManager& MediaManager;
	CResourceManager& ResourceManager;
	CFontManager& FontManager;
	CShaderManager& ShaderManager;
	CMatrixManager& MatrixManager;
	CEventManager& EventManager;
	CSceneManager& SceneManager;
protected:
	/*
	 * Constructors and destructors
	 */
	Application();
	virtual ~Application();

	virtual void Exit();

private:
	/*
	 * Private functions
	 */
	//! Create the SDL window
	void CreateSDLWindow();
	//! Initialize good states for OpenGL
	void InitializeOpenGL();
	//! Main render loop
	void MainLoop();
	//! To manage SDL events
	void Event(SDL_Event& events);
	/*
	 * Virtual functions
	 */
	//! Update all scene objects
	virtual void OnUpdate(double deltaTime) = 0;
	virtual void OnEvent(SDL_Event& events) = 0;
	//! Make all initializations
	virtual void OnInitialize() = 0;
	//! Draw the scene
	virtual void OnRender() = 0;

	/*
	 * Static data
	 */
	static Application* s_Application; ///< application instance pointer

	/*
	 * Privates Attributes
	 */
	/// SDL Attributes
	SDL_WindowID m_SDLFenetre;
	SDL_GLContext m_SDLOpenGLContext;
	// Graphic Device
	GraphicsDevice m_GraphicsDevice;
	/// To know how much time between to pass
	double m_TimeElapse;
	bool m_IsRunning; ///< If the application is on
};

}

#endif /* APPLICATION_H_ */
