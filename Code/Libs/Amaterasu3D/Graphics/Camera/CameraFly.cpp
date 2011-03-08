#include "CameraFly.h"
#include <iostream>

CameraFly::CameraFly(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up) :
	CameraAbstract(pos, target, up),
	m_angleX(0.0),
	m_angleY(0.0),
	m_factor_move(50.0),
	m_mouse_click(false),
	m_angleX_new(0.0),
	m_angleY_new(0.0)
{

}

CameraFly::~CameraFly()
{
}

void CameraFly::OnEvent(SDL_Event& event, double DeltaTime)
{
	if(event.type == SDL_KEYDOWN)
	{
		Math::CMatrix4 matrixTransform;
		const float factor = (DeltaTime*m_factor_move);
		 switch(event.key.keysym.sym)
		 {
		 	 case SDLK_a:
		 		 m_position += m_left * factor;
		 		 break;
		 	 case SDLK_d:
		 		m_position += -m_left * factor;
		 		break;
		 	case SDLK_w:
		 		m_position += m_forward * factor;
		 		break;
		 	case SDLK_s:
		 		m_position -= m_forward * factor;
		 		break;
		 }
	}
	else if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		m_mouse_click = true;
	}
	else if(event.type == SDL_MOUSEBUTTONUP)
	{
		m_mouse_click = false;
	}
	else if(event.type == SDL_MOUSEMOTION && m_mouse_click)
	{
		m_angleX_new = -event.motion.xrel;
		m_angleY_new = -event.motion.yrel;
		m_angleX -= m_angleY_new;
		m_angleY -= m_angleX_new;

		if (m_angleX > 89)
			m_angleX = 89;
		if (m_angleX < -89)
			m_angleX = -89;

	}
}

void CameraFly::ComputeMatrix()
{
	ComputeAngles();
	CameraAbstract::ComputeMatrix();
}

void CameraFly::ComputeAngles()
{
    m_up = Math::TVector3F(0,1,0);

	/*if ((m_angleX > 90 && m_angleX < 270)|| (m_angleX < -90 && m_angleX > -270))
		m_up = -1;
	else*/
		//m_up = 1;


	double r_temp = cos(-m_angleX*M_PI/180);
	m_forward.y = sin(-m_angleX*M_PI/180);
	m_forward.x = r_temp*cos(m_angleY*M_PI/180);
	m_forward.z = r_temp*sin(m_angleY*M_PI/180);



	m_left = m_up^m_forward;
	m_left.Normalize();

//	std::cout << "left " << m_left << std::endl;
//	std::cout << "forward " << m_forward << std::endl;
//	std::cout << "up " << m_forward << std::endl;

	m_target = m_position + m_forward;
}