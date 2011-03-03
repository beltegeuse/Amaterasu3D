#include <Graphics/SceneGraph/Group.h>

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
void Group::Draw(const Math::CMatrix4& matrix)
{
	DrawObject::Draw(matrix);
	for(unsigned int cpt=0 ; cpt<m_objects.size() ; cpt++)
	{
		m_objects[cpt]->Draw(GetCurrentMatrix());
	}
}
} // namespace

