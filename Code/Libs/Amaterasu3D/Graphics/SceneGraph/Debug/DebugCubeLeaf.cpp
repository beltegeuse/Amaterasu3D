#include "DebugCubeLeaf.h"
#include <Debug/OpenGLDebug.h>
#include <iostream>
#include <GL/gl.h>
#include <Enum.h>
#include <Graphics/GLSLShader.h>
#include <Logger/Logger.h>

GLfloat DebugCubeLeaf::CubeArray[24] = {
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f
};

GLfloat DebugCubeLeaf::CubeArrayColor[24] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f
};

GLuint DebugCubeLeaf::IndiceArray[36] = {
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		3,1,5,5,1,7,
		0,2,6,6,2,4,
		6,7,0,0,7,1,
		2,3,4,4,3,5
};


DebugCubeLeaf::DebugCubeLeaf()
{
	SetIndiceBuffer(IndiceArray, 36);
	SceneGraph::AssimpMeshBuffer buffer;
	buffer.buffer = CubeArray;
	buffer.dimension = 3;
	buffer.size = 24;
	buffer.owner = false;
	AddBuffer(buffer,VERTEX_ATTRIBUT);
	buffer.buffer = CubeArrayColor;
	AddBuffer(buffer,COLOR_ATTRIBUT);
	CompileBuffers();
}

DebugCubeLeaf::~DebugCubeLeaf()
{
}
