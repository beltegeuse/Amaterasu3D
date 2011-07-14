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

#ifndef EVENTLISTENERS_H_
#define EVENTLISTENERS_H_

#include <C3/Events.h>

namespace ama3D
{
class EventListener
{
public:
	virtual ~EventListener()
	{
	}
};

//XXX: Give the button
class MouseListener: public EventListener
{
public:
	MouseListener();
	virtual ~MouseListener();

	virtual void MousePressed() = 0;
	virtual void MouseReleased() = 0;
};

class KeyListener: public EventListener
{
public:
	KeyListener();
	virtual ~KeyListener();

	virtual void KeyPressed(C3::Key::Code& key) = 0;
	virtual void KeyReleased(C3::Key::Code& key) = 0;
};

class MouseMotionListener: public EventListener
{
public:
	MouseMotionListener();
	virtual ~MouseMotionListener();

	virtual void MouseMoved(int x, int y) = 0;
};

//XXX Heritage en diament.
class InputListener: public MouseListener,
		public KeyListener,
		public MouseMotionListener
{
public:
	InputListener();
	virtual ~InputListener();
};

class FrameListener: public EventListener
{
public:
	FrameListener();
	virtual ~FrameListener();

	//! delta represent the time between two rendering
	virtual void FrameStarted(double delta) = 0;
	virtual void FrameEnded() = 0;
};

//XXX: not implemented
class WindowListener: public EventListener
{
	WindowListener();
	virtual ~WindowListener();
};
}

#endif /* EVENTLISTENERS_H_ */
