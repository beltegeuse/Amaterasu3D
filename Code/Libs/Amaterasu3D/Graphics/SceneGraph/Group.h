#ifndef _Group_H
#define _Group_H

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/DrawObject.h>
#include <vector>

namespace SceneGraph
{

	/** \brief Noeud representant un groupe d'objets */
	class Group : public DrawObject
	{
	private:
		std::vector<DrawObject*> m_objects ;

	public:
		/** \brief Constructeur d'un noeud group */
		Group();
		virtual ~Group();
		/** \brief Recuperation des fils */
		const std::vector<DrawObject*> & GetChilds() const;
		/** \brief Ajout d'un fils au groupe d'objets. */
		virtual void AddChild(DrawObject * object);
		/** \brief Gestion de l'affichage OpenGL */
		virtual void Draw(const Math::CMatrix4& matrix);

	} ;

}

#endif
