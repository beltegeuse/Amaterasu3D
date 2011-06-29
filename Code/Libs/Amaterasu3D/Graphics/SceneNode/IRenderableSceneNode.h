/*
 * IRenderableSceneNode.h
 *
 *  Created on: Jun 29, 2011
 *      Author: adrien
 */

#ifndef IRENDERABLESCENENODE_H_
#define IRENDERABLESCENENODE_H_

#include <Graphics/SceneNode/ISceneNode.h>

class IRenderableSceneNode : public ISceneNode
{
public:
	IRenderableSceneNode(const std::string& name, ISceneNode* parent);
	virtual ~IRenderableSceneNode();

	virtual void Render() = 0;
};

#endif /* IRENDERABLESCENENODE_H_ */
