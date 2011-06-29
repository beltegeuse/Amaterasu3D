/*
 * ISimpleRenderableSceneNode.h
 *
 *  Created on: Jun 29, 2011
 *      Author: adrien
 */

#ifndef ISIMPLERENDERABLESCENENODE_H_
#define ISIMPLERENDERABLESCENENODE_H_

#include <Graphics/SceneNode/IRenderableSceneNode.h>
#include <Graphics/RenderableObject.h>

class ISimpleRenderableSceneNode : public IRenderableSceneNode
{
private:
	RenderableObject m_RenderObject;
public:
	ISimpleRenderableSceneNode();
	virtual ~ISimpleRenderableSceneNode();

	virtual void Render();
};

#endif /* ISIMPLERENDERABLESCENENODE_H_ */
