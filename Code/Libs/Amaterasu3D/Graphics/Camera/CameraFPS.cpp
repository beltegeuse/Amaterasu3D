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
#include "CameraFPS.h"
#include <iostream>
#include <Math/SphericalCoordinates.h>

namespace ama3D
{
CameraFPS::CameraFPS(const  glm::vec3& pos, const  glm::vec3& target,
		const  glm::vec3& up) :
		CameraAbstract(pos, target, up), m_angleX(0.0), m_angleY(0.0), m_mouse_click(
				false), m_angleX_new(0.0), m_angleY_new(0.0), m_factor_move(
				10.0), m_up_press(false), m_down_press(false), m_left_press(
				false), m_right_press(false)
{
//	Math::SphericalCoordinates s(target-pos);
//	m_angleX = s.GetPhy() * 180.0/M_PI;
//	m_angleY = s.GetTheta() * 180.0/M_PI;
//	ComputeAngles();
}

CameraFPS::~CameraFPS()
{
}

void CameraFPS::SetSpeed(float moveSec)
{
	m_factor_move = moveSec;
}

void CameraFPS::MousePressed()
{
	m_mouse_click = true;
}

void CameraFPS::MouseReleased()
{
	m_mouse_click = false;
}

void CameraFPS::KeyPressed(C3::Key::Code& key)
{
	switch (key)
	{
	case C3::Key::A:
		m_left_press = true;
		break;
	case C3::Key::D:
		m_right_press = true;
		break;
	case C3::Key::W:
		m_up_press = true;
		break;
	case C3::Key::S:
		m_down_press = true;
		break;
	default:
		break;
	}
}

void CameraFPS::KeyReleased(C3::Key::Code& key)
{
	switch (key)
	{
	case C3::Key::A:
		m_left_press = false;
		break;
	case C3::Key::D:
		m_right_press = false;
		break;
	case C3::Key::W:
		m_up_press = false;
		break;
	case C3::Key::S:
		m_down_press = false;
		break;
	default:
		break;
	}
}

void CameraFPS::MouseMoved(int x, int y)
{
	if (m_mouse_click)
	{
		m_angleX_new = -x;
		m_angleY_new = -y;
		m_angleX -= m_angleY_new;
		m_angleY -= m_angleX_new;

		if (m_angleX > 89)
			m_angleX = 89;
		if (m_angleX < -89)
			m_angleX = -89;
	}
}

void CameraFPS::FrameStarted(double delta)
{
	const float factor = (delta * m_factor_move);
	// Motion
	if (m_left_press)
		m_Position += m_left * factor;
	if (m_right_press)
		m_Position += -m_left * factor;
	if (m_up_press)
		m_Position += m_forward * factor;
	if (m_down_press)
		m_Position -= m_forward * factor;
	ComputeAngles();
	CameraAbstract::ComputeMatrix();
}

void CameraFPS::ComputeAngles()
{
	m_Up =  glm::vec3(0, 1, 0);

	/*if ((m_angleX > 90 && m_angleX < 270)|| (m_angleX < -90 && m_angleX > -270))
	 m_up = -1;
	 else*/
	//m_up = 1;

	double r_temp = cos(-m_angleX * M_PI / 180);
	m_forward.y = sin(-m_angleX * M_PI / 180);
	m_forward.x = r_temp * cos(m_angleY * M_PI / 180);
	m_forward.z = r_temp * sin(m_angleY * M_PI / 180);

	m_left = m_Up ^ m_forward;
	glm::normalize(m_left);

//	std::cout << "left " << m_left << std::endl;
//	std::cout << "forward " << m_forward << std::endl;
//	std::cout << "up " << m_forward << std::endl;

	m_Target = m_Position + m_forward;
}
}
