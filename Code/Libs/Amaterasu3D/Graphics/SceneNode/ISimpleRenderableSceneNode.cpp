#include "ISimpleRenderableSceneNode.h"

#include <Graphics/MatrixManagement.h>

ISimpleRenderableSceneNode::ISimpleRenderableSceneNode(const std::string& name, ISceneNode* parent) :
IRenderableSceneNode(name, parent)
{
}

ISimpleRenderableSceneNode::~ISimpleRenderableSceneNode()
{
}

void ISimpleRenderableSceneNode::Render()
{
	CMatrixManager::Instance().PushMatrix(GetWorldTransformation());
	m_RenderObject.Draw();
	CMatrixManager::Instance().PopMatrix();
}

RenderableObject& ISimpleRenderableSceneNode::GetObject()
{
	return m_RenderObject;
}
