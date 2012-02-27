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

#ifndef FIXEDCAMERA_H_
#define FIXEDCAMERA_H_

#include <Graphics/Camera/CameraAbstract.h>
#include "glm/glm.hpp"

namespace ama3D
{
class FixedCamera: public CameraAbstract
{
public:
	FixedCamera(const  glm::vec3& pos, const  glm::vec3& target,
			const  glm::vec3& up =  glm::vec3(0, 1, 0));
	virtual ~FixedCamera();

	virtual void MousePressed()
	{
	}
	virtual void MouseReleased()
	{
	}
	virtual void KeyPressed(C3::Key::Code& key)
	{
	}
	virtual void KeyReleased(C3::Key::Code& key)
	{
	}
	virtual void MouseMoved(int x, int y)
	{
	}
	virtual void FrameStarted(double delta)
	{
		CameraAbstract::ComputeMatrix();
	}
	virtual void FrameEnded()
	{
	}
};
}

#endif /* FIXEDCAMERA_H_ */
