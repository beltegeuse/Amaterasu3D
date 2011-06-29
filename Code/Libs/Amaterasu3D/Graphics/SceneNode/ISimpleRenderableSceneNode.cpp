#include "ISimpleRenderableSceneNode.h"

ISimpleRenderableSceneNode::ISimpleRenderableSceneNode(const std::string& name, ISceneNode* parent) :
IRenderableSceneNode(name, parent)
{
}

ISimpleRenderableSceneNode::~ISimpleRenderableSceneNode()
{
}

void ISimpleRenderableSceneNode::Render()
{
	m_RenderObject.Draw();
}

RenderableObject& ISimpleRenderableSceneNode::GetObject()
{
	return m_RenderObject;
}
