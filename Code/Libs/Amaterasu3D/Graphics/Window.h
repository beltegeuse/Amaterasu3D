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
#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>
#include <string>
#include <Math/Vector2.h>
#include <Graphics/SceneGraph/Group.h>
#include <Graphics/Camera/CameraAbstract.h>
#include <Addons/Console/Console.h>
class Window
{
public:
	// To get the Console
	CConsole& Console;

protected:
	// Attributs
	SDL_WindowID m_fenetre;
	SDL_GLContext m_contexteOpenGL;
	bool m_isRunning;
	SceneGraph::Group m_root;
	CameraAbstract* m_camera;
	// TODO: Write into a class
	int m_FPS;
	double m_timeEslapse;

public:
	// Constructor & Destructor
	Window(const std::string& name, const Math::TVector2I& windowSize = Math::TVector2I(800,600), bool syncVertical = false);
	Window();

	virtual ~Window();

	// Public methods
	void Run();
	SceneGraph::Group& GetSceneRoot();

	// Virtual methods
	virtual void OnDraw(double delta);
	virtual void OnEvent(SDL_Event& events, double delta);
	//! Warning, stole the adress value
	void SetCamera(CameraAbstract* camera);

private:
	// To initialise the rendering window
	void CreateWindow(const std::string& name, const Math::TVector2I& windowSize, bool syncVertical);
};

#endif /* WINDOW_H_ */
