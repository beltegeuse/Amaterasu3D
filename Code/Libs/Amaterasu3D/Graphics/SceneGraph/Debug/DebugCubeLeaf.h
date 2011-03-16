#ifndef DEBUGCUBELEAF_H_
#define DEBUGCUBELEAF_H_

#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/SceneGraph/DrawObject.h>
#include <GL/glew.h>
#include <GL/gl.h>

class DebugCubeLeaf : public SceneGraph::AssimpMesh
{
private:
	// Dessin de la geometrie
	static GLfloat CubeArray[24];
	static GLfloat CubeArrayColor[24];
	static GLuint IndiceArray[36];
public:
	DebugCubeLeaf();
	virtual ~DebugCubeLeaf();
};

#endif /* DEBUGCUBELEAF_H_ */
