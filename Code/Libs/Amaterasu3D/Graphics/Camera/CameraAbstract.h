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
#include <iostream>

#include <System/EventListeners.h>

///////////////////////////////////////////////////
/// This class describe an Abstract camera.
/// Use this class to create camera easily.
///////////////////////////////////////////////////
class CameraAbstract : public MouseMotionListener, public MouseListener, public KeyListener, public FrameListener
{
protected:
	/*
	 * Attributes
	 */
	Math::CMatrix4 m_ViewMatrixTransform; ///< Matrix correspond to the View transform matrix
	// * Pour le GluLookAt
	Math::TVector3F m_Position; ///< Camera position
	Math::TVector3F m_Target; ///< Camera direction
	Math::TVector3F m_Up; ///< Camera orientation
public:
	/*
	 * Constructors & Destructors
	 */
	CameraAbstract(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up = Math::TVector3F(0,0,1));
	virtual ~CameraAbstract();

	/*
	 * Public methods
	 */
	// Methods to manage View matrix
	void SetMatrix(const Math::CMatrix4& matrix);
	const Math::CMatrix4& GetMatrix();
	//! method to update the m_ViewMatrixTransform
	void ComputeMatrix();
	//! method to send the m_ViewMatrixTransform to the MatrixManager
	void GetView();
	// Methods to get somes imformations about the camera
	const Math::TVector3F& GetPosition() const;
	const Math::TVector3F& GetTarget() const;
	// Methods to configure the camera
	void SetPosition(const Math::TVector3F& position);
	void SetTarget(const Math::TVector3F& target);
	/*
	 * Abstract methods (heritated by Listeners)
	 */
	virtual void MousePressed() = 0;
	virtual void MouseReleased() = 0;
	virtual void KeyPressed(SDL_Keycode& key) = 0;
	virtual void KeyReleased(SDL_Keycode& key) = 0;
	virtual void MouseMoved(int x, int y) = 0;
	virtual void FrameStarted(double delta) = 0;
	virtual void FrameEnded() = 0;
};

inline std::ostream& operator<< (std::ostream& out, const CameraAbstract& cam)
{
	out << "Position : " << cam.GetPosition() << " | Target : " << cam.GetTarget();
	return out;
}

#endif /* CAMERAABSTRACT_H_ */
