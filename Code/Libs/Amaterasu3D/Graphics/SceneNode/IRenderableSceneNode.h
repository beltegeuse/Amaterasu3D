/*
 * IRenderableSceneNode.h
 *
 *  Created on: Jun 29, 2011
 *      Author: adrien
 */

#ifndef IRENDERABLESCENENODE_H_
#define IRENDERABLESCENENODE_H_

class IRenderableSceneNode {
public:
	IRenderableSceneNode();
	virtual ~IRenderableSceneNode();

	virtual void Render() = 0;
};

#endif /* IRENDERABLESCENENODE_H_ */
