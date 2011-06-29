#include "IMeshSceneNode.h"
#include <System/MediaManager.h>

IMeshSceneNode::IMeshSceneNode(const std::string& name, ISceneNode* parent) :
IRenderableSceneNode(name, parent),
m_scene(0)
{
}

IMeshSceneNode::~IMeshSceneNode()
{
	//FIXME: Faire le release ...
	if(!m_scene)
	{
		aiReleaseImport(m_scene);
	}
}

void IMeshSceneNode::SetAssimpScene(const aiScene* scene)
{
	m_scene = scene;
}

void IMeshSceneNode::AddRenderableObject(RenderableObject* obj, const Math::CMatrix4& trans)
{
	m_Objects[obj] = trans;
}

IMeshSceneNode* IMeshSceneNode::LoadFromFile(const std::string& filename, const std::string& name, ISceneNode* parent)
{
//	IMeshSceneNode* Resource = CResourceManager::Instance().Get<IMeshSceneNode>(filename);
//	if(Resource == NULL)
//	{
	return CMediaManager::Instance().LoadMediaFromFile<IMeshSceneNode>(filename);
//		CResourceManager::Instance().Add(filename, Resource);
//	}
//	return Resource;
}
