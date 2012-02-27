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

#include "IMeshSceneNode.h"
#include <System/MediaManager.h>

namespace ama3D
{
IMeshSceneNode::IMeshSceneNode(const std::string& name, ISceneNode* parent) :
		IRenderableSceneNode(name, parent), m_scene(0)
{
}

IMeshSceneNode::~IMeshSceneNode()
{
	//FIXME: Faire le release ...
	if (!m_scene)
	{
		aiReleaseImport(m_scene);
	}

	for (RenderableWorldObjectsList::iterator it = m_Objects.begin();
			it != m_Objects.end(); ++it)
		delete it->first;

	m_Objects.clear();
}

void IMeshSceneNode::SetAssimpScene(const aiScene* scene)
{
	m_scene = scene;
}

IMeshSceneNode::RenderableWorldObjectsList& IMeshSceneNode::GetRenderableObjects()
{
	return m_Objects;
}

void IMeshSceneNode::AddRenderableObject(RenderableObject* obj,
		const glm::mat4x4& trans)
{
	m_Objects[obj] = trans;
}

IMeshSceneNode* IMeshSceneNode::LoadFromFile(const std::string& filename,
		const std::string& name, ISceneNode* parent)
{
//	IMeshSceneNode* Resource = CResourceManager::Instance().Get<IMeshSceneNode>(filename);
//	if(Resource == NULL)
//	{
	return CMediaManager::Instance().LoadMediaFromFile<IMeshSceneNode>(filename);
//		CResourceManager::Instance().Add(filename, Resource);
//	}
//	return Resource;
}

void IMeshSceneNode::Render()
{
	CMatrixManager& mm = CMatrixManager::Instance();
	for (RenderableWorldObjectsList::iterator it = m_Objects.begin();
			it != m_Objects.end(); ++it)
			{
		glm::mat4x4 transMat = it->second
				* m_CachedWorldTransformationMatrix;
		mm.PushMatrix(transMat);
		it->first->Draw();
		mm.PopMatrix();
	}
}
}
