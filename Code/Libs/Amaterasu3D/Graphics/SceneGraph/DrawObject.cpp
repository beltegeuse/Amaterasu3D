#include <Graphics/SceneGraph/DrawObject.h>

namespace SceneGraph
{

DrawObject::DrawObject()
{
}
DrawObject::~DrawObject()
{
}
void DrawObject::Draw(const Math::CMatrix4& matrix)
{
	m_matrix = m_matrix_transform*matrix;
}
void DrawObject::LoadTransformMatrix(const Math::CMatrix4& matrix)
{
	m_matrix_transform = matrix;
}
void DrawObject::MultTransformMatrix(const Math::CMatrix4& matrix)
{
	m_matrix_transform = m_matrix_transform*matrix;
}
const Math::CMatrix4& DrawObject::GetTransformMatrix() const
{
	return m_matrix_transform;
}
const Math::CMatrix4& DrawObject::GetCurrentMatrix() const
{
	return m_matrix;
}

}
