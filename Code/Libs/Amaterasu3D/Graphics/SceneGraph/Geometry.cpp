/*
 * Geometry.cpp
 *
 *  Created on: Mar 3, 2011
 *      Author: Adrien
 */

#include "Geometry.h"

namespace SceneGraph
{
Geometry::Geometry()
{
}
Geometry::~Geometry()
{
}
void Geometry::Draw(const Math::CMatrix4& matrix)
{
	DrawObject::Draw(matrix);
	//FIXME: implement it
}
}
