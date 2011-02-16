#ifndef _DrawObject_H
#define _DrawObject_H

#include <Math/Matrix4.h>

namespace SceneGraph
{

	/** \brief Classe m�re de tous les noeuds du graphe de sc�ne. */
	class DrawObject
	{
	public :
		DrawObject();
		/** \brief Destructeur virtuel */
		virtual ~DrawObject();
		/** Gestion de l'affichage OpenGL */
		virtual void draw() = 0 ;
	} ;

}

#endif 
