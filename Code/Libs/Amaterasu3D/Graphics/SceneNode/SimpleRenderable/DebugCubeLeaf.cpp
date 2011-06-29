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
#include "DebugCubeLeaf.h"
#include <Debug/OpenGLDebug.h>
#include <iostream>
#include <GL/gl.h>
#include <Enum.h>
#include <Logger/Logger.h>

GLfloat DebugCubeLeaf::CubeArray[24] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
};

GLfloat DebugCubeLeaf::CubeArrayColor[24] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f
};

GLuint DebugCubeLeaf::IndiceArray[36] = {
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		3,1,5,5,1,7,
		0,2,6,6,2,4,
		6,7,0,0,7,1,
		2,3,4,4,3,5
};


DebugCubeLeaf::DebugCubeLeaf(const std::string& name, ISceneNode* parent) :
	ISimpleRenderableSceneNode(name, parent)
{
	m_RenderObject.SetIndiceBuffer(IndiceArray, 36);
	RenderableObject::RenderableBuffer buffer;
	buffer.buffer = CubeArray;
	buffer.dimension = 3;
	buffer.size = 24;
	buffer.owner = false;
	m_RenderObject.AddBuffer(buffer,VERTEX_ATTRIBUT);
	buffer.buffer = CubeArrayColor;
	m_RenderObject.AddBuffer(buffer,COLOR_ATTRIBUT);
	m_RenderObject.CompileBuffers();
}

DebugCubeLeaf::~DebugCubeLeaf()
{
}
