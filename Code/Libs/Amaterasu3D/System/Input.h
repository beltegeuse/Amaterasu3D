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

#ifndef INPUT_H_
#define INPUT_H_

#include <SDL/SDL.h>

/* Classe Input (de fvitman du SDZ) */
class Input
{
public:
	Input();
	~Input()
	{
	}
	void Update();
	inline bool Key(int i)
	{
		return key[i];
	}
	inline int MouseX()
	{
		return mousex;
	}
	inline int MouseY()
	{
		return mousey;
	}
	inline int MouseXrel()
	{
		return mousexrel;
	}
	inline int MouseYrel()
	{
		return mouseyrel;
	}
	inline bool MouseButton(int i)
	{
		return mousebuttons[i];
	}
	inline bool Quit()
	{
		return quit;
	}

protected:
	bool key[SDLK_LAST];
	int mousex, mousey;
	int mousexrel, mouseyrel;
	bool mousebuttons[8];
	bool quit;
};

Input::Input()
{
	memset(this, 0, sizeof(*this));
}

void Input::Update()
{
	SDL_Event event;
	mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			key[event.key.keysym.sym] = 1;
			break;
		case SDL_KEYUP:
			key[event.key.keysym.sym] = 0;
			break;
		case SDL_MOUSEMOTION:
			mousex = event.motion.x;
			mousey = event.motion.y;
			mousexrel = event.motion.xrel;
			mouseyrel = event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mousebuttons[event.button.button] = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button != SDL_BUTTON_WHEELUP
					&& event.button.button != SDL_BUTTON_WHEELDOWN
				)
				mousebuttons[event.button.button] = 0;
			break;
		case SDL_QUIT:
			quit = 1;
			break;
		default:
			break;
		}
	}
}

#endif /* INPUT_H_ */
