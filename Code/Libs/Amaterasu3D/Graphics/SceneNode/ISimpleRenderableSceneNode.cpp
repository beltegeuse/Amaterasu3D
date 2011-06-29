#include "ISimpleRenderableSceneNode.h"

ISimpleRenderableSceneNode::ISimpleRenderableSceneNode()
{
}

ISimpleRenderableSceneNode::~ISimpleRenderableSceneNode()
{
}

void ISimpleRenderableSceneNode::Render()
{
	m_RenderObject.Draw();
}
