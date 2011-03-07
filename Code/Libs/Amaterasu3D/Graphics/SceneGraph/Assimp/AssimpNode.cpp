/*
 * AssimpNode.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Adrien
 */

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
