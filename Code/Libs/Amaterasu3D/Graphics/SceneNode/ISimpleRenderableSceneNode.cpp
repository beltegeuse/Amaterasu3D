//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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
#include "ISimpleRenderableSceneNode.h"

#include <Graphics/MatrixManagement.h>
#include <iostream>

namespace ama3D
{
ISimpleRenderableSceneNode::ISimpleRenderableSceneNode(const std::string& name,
		ISceneNode* parent) :
		IRenderableSceneNode(name, parent)
{
}

ISimpleRenderableSceneNode::~ISimpleRenderableSceneNode()
{
}

void ISimpleRenderableSceneNode::Render()
{
	//std::cout << GetWorldTransformation() << std::endl;
	CMatrixManager::Instance().PushMatrix(GetWorldTransformation());
	m_RenderObject.Draw();
	CMatrixManager::Instance().PopMatrix();
}

RenderableObject& ISimpleRenderableSceneNode::GetObject()
{
	return m_RenderObject;
}
}
