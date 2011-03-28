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
#include "Rectangle2D.h"

Rectangle2D::Rectangle2D(const Math::TVector2I& top, const Math::TVector2I& bottom, bool generateUVCoord)
{
	// Create Vertex buffer
	float* vertexBuffer = new float[8];
	vertexBuffer[0] = top.x;
	vertexBuffer[1] = bottom.y;
	vertexBuffer[2] = top.x;
	vertexBuffer[3] = top.y;
	vertexBuffer[4] = bottom.x;
	vertexBuffer[5] = top.y;
	vertexBuffer[6] = bottom.x;
	vertexBuffer[7] = bottom.y;
	SceneGraph::ModelBuffer buffer;
	buffer.buffer = vertexBuffer;
	buffer.dimension = 2;
	buffer.size = 8;
	buffer.owner = true;
	AddBuffer(buffer, VERTEX_ATTRIBUT);
	// Create Texcoord buffer
	if(generateUVCoord)
	{
		float* uvBuffer = new float[8];
		uvBuffer[0] = 0;
		uvBuffer[1] = 0;
		uvBuffer[2] = 0;
		uvBuffer[3] = 1;
		uvBuffer[4] = 1;
		uvBuffer[5] = 1;
		uvBuffer[6] = 1;
		uvBuffer[7] = 0;
		buffer.buffer = uvBuffer;
		AddBuffer(buffer,TEXCOORD_ATTRIBUT);
	}
	// Create Index buffer
	unsigned int* indiceBuffer = new unsigned int[6];
	indiceBuffer[0] = 0;
	indiceBuffer[1] = 1;
	indiceBuffer[2] = 2;
	indiceBuffer[3] = 0;
	indiceBuffer[4] = 2;
	indiceBuffer[5] = 3;
	SetIndiceBuffer(indiceBuffer, 6);
	CompileBuffers();
}

Rectangle2D::~Rectangle2D()
{
}