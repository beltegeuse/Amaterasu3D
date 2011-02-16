//==========================================================
// Yes::Engine - Free C++ 3D engine
//
// Copyright (C) 2004-2005 Laurent Gomila
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : laurent.gom@gmail.com
//==========================================================

#ifndef RECTANGLE_H
#define RECTANGLE_H

//==========================================================
// En-tetes
//==========================================================
#include <Math/Vector2.h>
#include <iostream>


// TODO : mettre ailleurs !
////////////////////////////////////////////////////////////
/// Types d'intersections
////////////////////////////////////////////////////////////
enum TIntersection
{
    INT_IN,        ///< Completement a l'interieur
    INT_OUT,       ///< Completement a l'exterieur
    INT_INTERSECTS ///< Intersection
};


namespace Math
{
    ////////////////////////////////////////////////////////////
    /// Class des rectangles 2D a coordonnees entieres
    ////////////////////////////////////////////////////////////
    class CRectangle
    {
    public :

        //----------------------------------------------------------
        // Constructeur par defaut
        //----------------------------------------------------------
        CRectangle(const TVector2I& Start = TVector2I(0, 0), const TVector2I& Size = TVector2I(0, 0));

        //----------------------------------------------------------
        // Constructeur e partir de 2 coordonnees et 2 dimensions
        //----------------------------------------------------------
        CRectangle(int Left, int Top, int Width, int Height);

        //----------------------------------------------------------
        // Reinitialise le rectangle
        //----------------------------------------------------------
        void Set(int Left, int Top, int Width, int Height);

        //----------------------------------------------------------
        // Renvoie le cote gauche du rectangle
        //----------------------------------------------------------
        int Left() const;

        //----------------------------------------------------------
        // Renvoie le cote droit du rectangle
        //----------------------------------------------------------
        int Right() const;

        //----------------------------------------------------------
        // Renvoie le cote haut du rectangle
        //----------------------------------------------------------
        int Top() const;

        //----------------------------------------------------------
        // Renvoie le cote bas du rectangle
        //----------------------------------------------------------
        int Bottom() const;

        //----------------------------------------------------------
        // Renvoie la largeur du rectangle
        //----------------------------------------------------------
        int Width() const;

        //----------------------------------------------------------
        // Renvoie la hauteur du rectangle
        //----------------------------------------------------------
        int Height() const;

        //----------------------------------------------------------
        // Renvoie la taille du rectangle
        //----------------------------------------------------------
        TVector2I Size() const;

        //----------------------------------------------------------
        // Test d'intersection avec un point
        //----------------------------------------------------------
        TIntersection Intersects(const TVector2I& Point) const;

        //----------------------------------------------------------
        // Test d'intersection avec un rectangle
        //----------------------------------------------------------
        TIntersection Intersects(const CRectangle& Rect) const;

        //----------------------------------------------------------
        // Operateurs de comparaison
        //----------------------------------------------------------
        bool operator ==(const CRectangle& Rect) const;
        bool operator !=(const CRectangle& Rect) const;

        //----------------------------------------------------------
        // Donnees membres
        //----------------------------------------------------------
        TVector2I Origin; ///< Coin haut-gauche
        TVector2I End;    ///< Coin bas-droit
    };

    //==========================================================
    // Fonctions globales relatives aux rectangles
    //==========================================================
    std::istream& operator >>(std::ostream& Stream, CRectangle& Rect);
    std::ostream& operator <<(std::ostream& Stream, const CRectangle& Rect);

} // namespace Yes


#endif // RECTANGLE_H
