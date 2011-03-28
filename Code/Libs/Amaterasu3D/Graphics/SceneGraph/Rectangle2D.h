//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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
// E-mail : adrien.gruson@gmail.com
//==========================================================

#ifndef RECTANGLE2D_H_
#define RECTANGLE2D_H_

#include <Graphics/SceneGraph/Model.h>
#include <Math/Vector2.h>

class Rectangle2D : public SceneGraph::Model
{
public:
	/*
	 * Constructors & Destructors
	 */
	Rectangle2D(const Math::TVector2I& top, const Math::TVector2I& bottom, bool generateUVCoord = true);
	virtual ~Rectangle2D();
};

#endif /* RECTANGLE2D_H_ */
