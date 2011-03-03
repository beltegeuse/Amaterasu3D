/*
 * Geometry.h
 *
 *  Created on: Mar 3, 2011
 *      Author: Adrien
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <Graphics/SceneGraph/DrawObject.h>

namespace SceneGraph
{
class Geometry : public DrawObject
{
public:
	Geometry();
	virtual ~Geometry();
	virtual void Draw(const Math::CMatrix4& matrix);

};
}

#endif /* GEOMETRY_H_ */
