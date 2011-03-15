#include "MatrixManagement.h"
#include <Debug/Exceptions.h>

SINGLETON_IMPL(MatrixManagement)

MatrixManagement::MatrixManagement(int maxMatrix) :
m_maxMatrix(maxMatrix)
{
	m_identityMatrix.Identity();
}

MatrixManagement::~MatrixManagement()
{
	if(!m_matrix.empty())
		std::cout << "[WARNING] There still somes matrix in the stack..." << std::endl;
}

void MatrixManagement::PushMatrix(const Math::CMatrix4& matrix)
{
	// Debug limit
	if(m_maxMatrix < m_matrix.size())
		throw CException("Matrix stack is full.");

	// stack the new matrix
	if(m_matrix.empty())
		m_matrix.push_back(matrix);
	else
		m_matrix.push_back(m_matrix.back()*matrix);

	m_signal_event.emit(MODEL_MATRIX);
	m_signal_event.emit(NORMAL_MATRIX);

}

void MatrixManagement::PopMatrix()
{
	if(m_matrix.empty())
		throw CException("Matrix stack is empty. Unable to pop matrix");
	m_matrix.pop_back();

	m_signal_event.emit(MODEL_MATRIX);
	m_signal_event.emit(NORMAL_MATRIX);
}

bool MatrixManagement::IsEmpty() const
{
	return m_matrix.empty();
}

int MatrixManagement::StackSize() const
{
	return m_matrix.size();
}

const Math::CMatrix4& MatrixManagement::GetMatrix(MatrixType type)
{
	if(type == MODEL_MATRIX)
	{
		if(m_matrix.empty())
			return m_identityMatrix;
		return m_matrix.back();
	}
	else if(type == VIEW_MATRIX)
	{
		return m_viewMatrix;
	}
	else if(type == PROJECTION_MATRIX)
	{
		return m_projectionMatrix;
	}
	else if(type == NORMAL_MATRIX)
	{
		// Update
		m_normalMatrix = GetMatrix(MODEL_MATRIX).Inverse().Transpose();
		return m_normalMatrix;
	}
	else
	{
		throw CException("Unknow type");
	}
}

void MatrixManagement::SetProjectionMatrix(const Math::CMatrix4& matrix)
{
	m_projectionMatrix = matrix;
	m_signal_event.emit(PROJECTION_MATRIX);
}

void MatrixManagement::SetViewMatrix(const Math::CMatrix4& matrix)
{
	m_viewMatrix = matrix;
	m_signal_event.emit(VIEW_MATRIX);
}

sigc::signal<void, MatrixType>& MatrixManagement::GetSignalEvent()
{
	return m_signal_event;
}

