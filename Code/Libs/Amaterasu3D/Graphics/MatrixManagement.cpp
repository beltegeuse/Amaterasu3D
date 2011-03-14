#include "MatrixManagement.h"
#include <Debug/Exceptions.h>

SINGLETON_IMPL(MatrixManagement)

MatrixManagement::MatrixManagement(int maxMatrix) :
m_maxMatrix(maxMatrix)
{
	m_identityMatrix.Identity();
	//XXX May be initialise the array for faster access
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

	//TODO: deprecedied functions
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf((const float*)matrix);

	// stack the new matrix
	if(m_matrix.empty())
		m_matrix.push_back(matrix);
	else
		m_matrix.push_back(matrix*m_matrix.back());

	// TODO: to debug
	Math::CMatrix4 matrixOpenGL;
	matrixOpenGL.LoadFromOpenGL(GL_MODELVIEW_MATRIX);
	m_signal_event.emit(MODELVIEW_MATRIX, matrixOpenGL);

}

void MatrixManagement::PopMatrix()
{
	if(m_matrix.empty())
		throw CException("Matrix stack is empty. Unable to pop matrix");
	m_matrix.pop_back();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// TODO: to debug
	Math::CMatrix4 matrixOpenGL;
	matrixOpenGL.LoadFromOpenGL(GL_MODELVIEW_MATRIX);
	m_signal_event.emit(MODELVIEW_MATRIX, matrixOpenGL);
}

bool MatrixManagement::IsEmpty() const
{
	return m_matrix.empty();
}

int MatrixManagement::StackSize() const
{
	return m_matrix.size();
}

const Math::CMatrix4& MatrixManagement::GetMatrix() const
{
	if(m_matrix.empty())
		return m_identityMatrix;
	return m_matrix.back();
}

void MatrixManagement::SetProjectionMatrix(const Math::CMatrix4& matrix)
{
	m_projectionMatrix = matrix;
	m_signal_event.emit(PROJECTION_MATRIX, matrix);
}

const Math::CMatrix4& MatrixManagement::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

void MatrixManagement::SetViewMatrix(const Math::CMatrix4& matrix)
{
	m_viewMatrix = matrix;
}

const Math::CMatrix4& MatrixManagement::GetViewMatrix() const
{
	return m_viewMatrix;
}

sigc::signal<void, MatrixType, const Math::CMatrix4&>& MatrixManagement::GetSignalEvent()
{
	return m_signal_event;
}

