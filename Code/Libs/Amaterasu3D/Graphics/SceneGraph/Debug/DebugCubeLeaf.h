/*
 * DebugCubeLeaf.h
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#ifndef DEBUGCUBELEAF_H_
#define DEBUGCUBELEAF_H_

#include <Graphics/SceneGraph/DrawObject.h>
#include <GL/glew.h>
#include <GL/gl.h>

class DebugCubeLeaf : public SceneGraph::DrawObject
{
private:
	// Dessin de la geometrie
	static GLfloat CubeArray[48];
	static GLuint IndiceArray[36];
	unsigned int m_cubeBuffers[2];
public:
	DebugCubeLeaf();
	virtual ~DebugCubeLeaf();

	virtual void Draw();
};

#endif /* DEBUGCUBELEAF_H_ */
