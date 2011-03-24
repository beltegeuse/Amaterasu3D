//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================
#include "DebugPlaneLeaf.h"

#include <Debug/OpenGLDebug.h>
#include <iostream>
#include <GL/gl.h>
#include <Enum.h>
#include <Graphics/GLSLShader.h>
#include <Logger/Logger.h>


namespace SceneGraph
{

GLfloat DebugPlaneLeaf::PlaneArray[24] = {
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f,
		1.0f, 1.0f, 1.0f, -1.0f,0.0f, -1.0f,
		0.0f, 0.0f, 1.0f, -1.0f,0.0f, 1.0f,
	};

GLuint DebugPlaneLeaf::IndiceArray[6] = {
		0,1,2,2,1,3
		};


DebugPlaneLeaf::DebugPlaneLeaf()
{
	// Génération des buffers
	Logger::Log() << "[INFO] Gen Buffer ...\n";
	GLCheck(glGenBuffers( 2, m_planebuffers ));
	// Buffer d'informations de vertex
	Logger::Log() << "[INFO] Fill Array Buffer ...\n";
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_planebuffers[0]));
	GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneArray), PlaneArray, GL_STATIC_DRAW));
	// Buffer d'indices
	Logger::Log() << "[INFO] Fill Element Array Buffer ...\n";
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planebuffers[1]));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndiceArray), IndiceArray, GL_STATIC_DRAW));
}

DebugPlaneLeaf::~DebugPlaneLeaf()
{
	glDeleteBuffers(2, m_planebuffers);
}

void DebugPlaneLeaf::Draw()
{
	// pas de shader
	if(!glShaderManager::Instance().activedShader())
	{
		Logger::Log() << "[Warning] No actived shader. Nothings to render ... \n";
	}
	//  * Les differents blindings ...
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_planebuffers[0]));
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
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_planebuffers[1]));
	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	// Desactivation des buffers
	if(vertexSupport)
		GLCheck(glDisableVertexAttribArray ( VERTEX_ATTRIBUT ));
	if(colorSupport)
		GLCheck(glDisableVertexAttribArray ( COLOR_ATTRIBUT ));
}


} // Namespace SceneGraph
