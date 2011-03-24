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
#include "AssimpNode.h"
#include <System/ResourceManager.h>
#include <System/MediaManager.h>

namespace SceneGraph
{
AssimpNode::AssimpNode() :
		m_scene(NULL)
{

}

AssimpNode::~AssimpNode()
{
	//FIXME: Faire le release ...
	if(!m_scene)
	{
		aiReleaseImport(m_scene);
	}
}

void AssimpNode::SetAssimpScene(const aiScene* scene)
{
	m_scene = scene;
}

SceneGraph::AssimpNode* AssimpNode::LoadFromFile(const std::string& filename)
{
	SceneGraph::AssimpNode* Resource = CResourceManager::Instance().Get<SceneGraph::AssimpNode>(filename);
	if(Resource == NULL)
	{
		Resource = CMediaManager::Instance().LoadMediaFromFile<SceneGraph::AssimpNode>(filename);
		CResourceManager::Instance().Add(filename, Resource);
	}
	return Resource;
}

} // Namespace SceneGraph
