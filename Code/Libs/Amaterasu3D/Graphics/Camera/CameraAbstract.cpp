#include "CameraAbstract.h"
#include <Graphics/GLSLShader.h>
#include <Graphics/MatrixManagement.h>

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
//	std::cout << "[DEBUG] Look at : " << std::endl;
//	std::cout << "  * position : " << m_position << std::endl;
//	std::cout << "  * target : " << m_target << std::endl;
	m_matrix.LookAt(m_position, m_target);
	//m_matrix = m_matrix.Transpose();
}

void CameraAbstract::GetView()
{
	MatrixManagement::Instance().SetViewMatrix(m_matrix);
}

void CameraAbstract::SendInvMatrix()
{
	Math::CMatrix4 matrix;
	matrix.LookAt(m_position, m_target);
	matrix = matrix.Inverse();
	glShaderManager::Instance().currentShader()->setUniformMatrix4fv("InvViewMatrix", matrix);
}
