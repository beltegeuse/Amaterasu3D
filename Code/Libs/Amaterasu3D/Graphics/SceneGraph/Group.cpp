#include <Graphics/SceneGraph/Group.h>
#include <Graphics/MatrixManagement.h>
#include <Logger/Logger.h>
namespace SceneGraph
{
/** \brief Constructeur d'un noeud group */
Group::Group()
{
}
Group::~Group()
{
	for(unsigned int cpt=0, size=(unsigned int)m_objects.size() ; cpt<size ; ++cpt)
	{
		delete m_objects[cpt] ;
	}
}
/** \brief Recuperation des fils */
const std::vector<DrawObject*> & Group::GetChilds() const
{ return m_objects ; }
/** \brief Ajout d'un fils au groupe d'objets. */
void Group::AddChild(DrawObject * object)
{
	m_objects.push_back(object) ;
}
/** \brief Gestion de l'affichage OpenGL */
void Group::Draw()
{
//	Logger::Log() << "[DEBUG] Group::Draw\n";
	MatrixManagement::Instance().PushMatrix(m_matrix_transform);
	for(unsigned int cpt=0 ; cpt<m_objects.size() ; cpt++)
	{
		m_objects[cpt]->Draw();
	}
	MatrixManagement::Instance().PopMatrix();
}

void Group::LoadTransformMatrix(const Math::CMatrix4& matrix)
{
	m_matrix_transform = matrix;
}
void Group::MultTransformMatrix(const Math::CMatrix4& matrix)
{
	m_matrix_transform = m_matrix_transform*matrix;
}
const Math::CMatrix4& Group::GetTransformMatrix() const
{
	return m_matrix_transform;
}

} // namespace

