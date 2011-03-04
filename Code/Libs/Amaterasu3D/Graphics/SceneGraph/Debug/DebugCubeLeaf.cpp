/*
 * DebugCubeLeaf.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#include "DebugCubeLeaf.h"
#include <Debug/OpenGLDebug.h>
#include <iostream>
#include <GL/gl.h>

GLfloat DebugCubeLeaf::CubeArray[48] = {
		1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f,
		1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f
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
	// Génération des buffers
	std::cout << "[INFO] Gen Buffer ..." << std::endl;
	GLCheck(glGenBuffers( 2, m_cubeBuffers ));
	// Buffer d'informations de vertex
	std::cout << "[INFO] Fill Array Buffer ..." << std::endl;
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_cubeBuffers[0]));
	GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(CubeArray), CubeArray, GL_STATIC_DRAW));
	// Buffer d'indices
	std::cout << "[INFO] Fill Element Array Buffer ..." << std::endl;
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeBuffers[1]));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndiceArray), IndiceArray, GL_STATIC_DRAW));
}

DebugCubeLeaf::~DebugCubeLeaf()
{
	glDeleteBuffers(2, m_cubeBuffers);
}

void DebugCubeLeaf::Draw()
{
	//  * Les differents blindings ...
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_cubeBuffers[0]));
	GLCheck(glVertexPointer( 3, GL_FLOAT, 6 * sizeof(float), ((float*)NULL + (3)) ));
	GLCheck(glColorPointer( 3, GL_FLOAT, 6 * sizeof(float), 0 ));
	//GLCheck(glEnableVertexAttribArray(m_shader.GetAttribLocation("VertexPosition")));
	//GLCheck(glVertexAttribPointer (m_shader.GetAttribLocation("VertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0) );
	// * le dessins en lui meme
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeBuffers[1]));
	GLCheck(glEnableClientState( GL_VERTEX_ARRAY ));
	GLCheck(glEnableClientState( GL_COLOR_ARRAY ));
	GLCheck(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
	//GLCheck(glDisableVertexAttribArray(m_shader.GetAttribLocation("VertexPosition")));
	GLCheck(glDisableClientState( GL_COLOR_ARRAY ));
	GLCheck(glDisableClientState( GL_VERTEX_ARRAY ));
}
