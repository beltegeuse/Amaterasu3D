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

#include <System/EventListeners.h>
#include <System/EventManager.h>

MouseListener::MouseListener()
{
	CEventManager::Instance().AddListener("MouseListener", this);
}
MouseListener::~MouseListener()
{
	CEventManager::Instance().RemoveListener("MouseListener", this);
}

KeyListener::KeyListener()
{
	CEventManager::Instance().AddListener("KeyListener", this);
}
KeyListener::~KeyListener()
{
	CEventManager::Instance().RemoveListener("KeyListener", this);
}

MouseMotionListener::MouseMotionListener()
{
	CEventManager::Instance().AddListener("MouseMotionListener", this);
}
MouseMotionListener::~MouseMotionListener()
{
	CEventManager::Instance().RemoveListener("MouseMotionListener", this);
}

FrameListener::FrameListener()
{
	CEventManager::Instance().AddListener("FrameListener", this);
}
FrameListener::~FrameListener()
{
	CEventManager::Instance().RemoveListener("FrameListener", this);
}

WindowListener::WindowListener()
{
	CEventManager::Instance().AddListener("WindowListener", this);
}
WindowListener::~WindowListener()
{
	CEventManager::Instance().RemoveListener("WindowListener", this);
}

InputListener::InputListener()
{
}

InputListener::~InputListener()
{
}

