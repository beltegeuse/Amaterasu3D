#include "CameraAbstract.h"

CameraAbstract::CameraAbstract(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up) :
	m_position(pos),
	m_target(target),
	m_up(m_up)
{
}

CameraAbstract::~CameraAbstract()
{
}

const Math::CMatrix4& CameraAbstract::GetMatrix()
{
//	std::cout << m_matrix << std::endl;
	return m_matrix;
}

void CameraAbstract::SetMatrix(const Math::CMatrix4& matrix)
{
	m_matrix = matrix;
}

void CameraAbstract::ComputeMatrix(double delta)
{
	std::cout << "[DEBUG] Look at : " << std::endl;
	std::cout << "  * position : " << m_position << std::endl;
	std::cout << "  * target : " << m_target << std::endl;
	//m_matrix.LookAt(m_position, m_target);
	m_matrix.Identity();
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(m_position.x,m_position.y,m_position.z,
			  m_target.x, m_target.y, m_target.z,
			  0,1,0);
}
