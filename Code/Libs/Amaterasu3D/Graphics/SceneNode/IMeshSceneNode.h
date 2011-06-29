/*
 * IMeshSceneNode.h
 *
 *  Created on: Jun 9, 2011
 *      Author: adrien
 */

#ifndef IMESHSCENENODE_H_
#define IMESHSCENENODE_H_

#include <Graphics/SceneNode/ISceneNode.h>

class IMeshSceneNode : public ISceneNode
{
public:
	IMeshSceneNode(const std::string& name, ISceneNode* parent);
	virtual ~IMeshSceneNode();

	virtual void Render();
};

#endif /* IMESHSCENENODE_H_ */
