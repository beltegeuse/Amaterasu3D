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
#ifndef CAMERAFLY_H_
#define CAMERAFLY_H_

#include <Graphics/Camera/CameraAbstract.h>
#include <Math/Vector3.h>

class CameraFPS : public CameraAbstract
{
private:
	/*
	 * Attributes
	 */
	// Pour bouger...
	Math::TVector3F m_forward;
	Math::TVector3F m_left;

	float m_angleX;
	float m_angleY;

	// Les valeurs temporaires
	bool m_mouse_click;
	float m_angleX_new;
	float m_angleY_new;

	// Les constantes
	float m_factor_move;

	// For keyboard
	bool m_up_press;
	bool m_down_press;
	bool m_left_press;
	bool m_right_press;

public:
	/*
	 * Constructors & Destructors
	 */
	CameraFPS(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up = Math::TVector3F(0,1,0));
	virtual ~CameraFPS();

	/*
	 * Public methods
	 */
	//! To change the speed of the camera
	void SetSpeed(float moveSec);

	/*
	 * Listener methods
	 */
	virtual void MousePressed();
	virtual void MouseReleased();
	virtual void KeyPressed(SDL_Keycode& key);
	virtual void KeyReleased(SDL_Keycode& key);
	virtual void MouseMoved(int x, int y);
	virtual void FrameStarted(double delta);
	virtual void FrameEnded() {} // Doesn't need

private:
	void ComputeAngles();
};

#endif /* CAMERAFLY_H_ */
