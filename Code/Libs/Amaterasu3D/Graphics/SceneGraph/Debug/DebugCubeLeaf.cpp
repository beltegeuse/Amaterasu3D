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
#include <Enum.h>
#include <Graphics/GLSLShader.h>
#include <Logger/Logger.h>

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
	Logger::Log() << "[INFO] Gen Buffer ...\n";
	GLCheck(glGenBuffers( 2, m_cubeBuffers ));
	// Buffer d'informations de vertex
	Logger::Log() << "[INFO] Fill Array Buffer ...\n";
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_cubeBuffers[0]));
	GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(CubeArray), CubeArray, GL_STATIC_DRAW));
	// Buffer d'indices
	Logger::Log() << "[INFO] Fill Element Array Buffer ...\n";
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeBuffers[1]));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndiceArray), IndiceArray, GL_STATIC_DRAW));
}

DebugCubeLeaf::~DebugCubeLeaf()
{
	glDeleteBuffers(2, m_cubeBuffers);
}

void DebugCubeLeaf::Draw()
{
	// pas de shader
	if(!glShaderManager::Instance().activedShader())
	{
		Logger::Log() << "[Warning] No actived shader. Nothings to render ... \n";
	}
	//  * Les differents blindings ...
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_cubeBuffers[0]));
	// Les disponibilites du shaders
	bool vertexSupport = glShaderManager::Instance().currentShader()->attributAvailable(VERTEX_ATTRIBUT);
	bool colorSupport = glShaderManager::Instance().currentShader()->attributAvailable(COLOR_ATTRIBUT);

	if(!(vertexSupport || colorSupport))
	{
		// Nothings to render ...
		return;
	}
	// Activation des buffers
	if(vertexSupport)
	{
		glEnableVertexAttribArray (VERTEX_ATTRIBUT);
		GLCheck(glVertexAttribPointer(VERTEX_ATTRIBUT, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0));
	}
	if(colorSupport)
	{
		glEnableVertexAttribArray (COLOR_ATTRIBUT);
		GLCheck(glVertexAttribPointer(COLOR_ATTRIBUT, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), ((float*)NULL + (3))));
	}
	// * le dessins en lui meme
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeBuffers[1]));
	GLCheck(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
	// Desactivation des buffers
	if(vertexSupport)
		GLCheck(glDisableVertexAttribArray ( VERTEX_ATTRIBUT ));
	if(colorSupport)
		GLCheck(glDisableVertexAttribArray ( COLOR_ATTRIBUT ));
}
