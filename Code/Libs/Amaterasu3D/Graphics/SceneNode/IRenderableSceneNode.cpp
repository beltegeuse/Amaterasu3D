/*
 * IRenderableSceneNode.cpp
 *
 *  Created on: Jun 29, 2011
 *      Author: adrien
 */

#include "IRenderableSceneNode.h"

IRenderableSceneNode::IRenderableSceneNode(const std::string& name, ISceneNode* parent) :
	ISceneNode(name, parent)
{
}

IRenderableSceneNode::~IRenderableSceneNode() {
	// TODO Auto-generated destructor stub
}
