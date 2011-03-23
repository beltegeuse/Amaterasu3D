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
#ifndef CAMERAABSTRACT_H_
#define CAMERAABSTRACT_H_

#include <Math/Vector3.h>
#include <Math/Matrix4.h>
#include <SDL/SDL_events.h>

class CameraAbstract
{
protected:
	// Attributs
	Math::CMatrix4 m_matrix;
	// * Pour le GluLookAt
	Math::TVector3F m_position;
	Math::TVector3F m_target;
	Math::TVector3F m_up;
public:
	// Constructor & destructors
	CameraAbstract(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up = Math::TVector3F(0,0,1));
	virtual ~CameraAbstract();

	void SetMatrix(const Math::CMatrix4& matrix);
	virtual void ComputeMatrix(double delta);
	void GetView();
	void SendInvMatrix();
	const Math::CMatrix4& GetMatrix();
	virtual void OnEvent(SDL_Event& events, double deltaTime) = 0;
};

#endif /* CAMERAABSTRACT_H_ */
