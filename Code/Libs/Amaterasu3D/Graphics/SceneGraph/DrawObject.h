#ifndef _DrawObject_H
#define _DrawObject_H

#include <Math/Matrix4.h>

namespace SceneGraph
{

	/** \brief Classe m�re de tous les noeuds du graphe de sc�ne. */
	class DrawObject
	{
	protected:
		Math::CMatrix4 m_matrix;
		Math::CMatrix4 m_matrix_transform;

	public :
		DrawObject();
		/** \brief Destructeur virtuel */
		virtual ~DrawObject();
		/** Gestion de l'affichage OpenGL */
		virtual void Draw(const Math::CMatrix4& matrix);
		/**
		 * All transform matrix management
		 */
		void LoadTransformMatrix(const Math::CMatrix4& matrix);
		void MultTransformMatrix(const Math::CMatrix4& matrix);
		const Math::CMatrix4& GetTransformMatrix() const;
		const Math::CMatrix4& GetCurrentMatrix() const;
	} ;

}

#endif 
