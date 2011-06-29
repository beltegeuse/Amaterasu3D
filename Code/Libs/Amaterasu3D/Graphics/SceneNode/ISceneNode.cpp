#include "ISceneNode.h"

ISceneNode::ISceneNode(const std::string& name, ISceneNode* parent) : m_Name(name), m_Parent(parent)
{
	if(m_Parent)
		m_Parent->AddChild(this);
}
ISceneNode::~ISceneNode()
{
	// Detach form the scenegraph
	if(m_Parent)
		m_Parent->DetachChild(this);
}

/*
 * Getters
 */
const std::string& ISceneNode::GetName() const
{
	return m_Name;
}
ISceneNode* ISceneNode::GetParent()
{
	return m_Parent;
}

/*
 * Hierachie
 */
void ISceneNode::AddChild(ISceneNode* node)
{
	Assert(m_Children.find(node->GetName()) == m_Children.end());
	m_Children[node->GetName()] = node;
}
void ISceneNode::DetachChild(ISceneNode* node)
{
	m_Children.erase(node->GetName());
}
void ISceneNode::DeleteAllChildren()
{
	for(SceneNodeList::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
		delete (it->second);
	m_Children.erase(m_Children.begin(), m_Children.end());
}

/*
 * Transformation
 */
// Transformation management
bool ISceneNode::IsNeedTransformationHierachicalCacheUpdate() const
{
	return m_NeedUpdate;
}
// * Getters
const Math::CMatrix4 ISceneNode::GetLocalTransformation() const
{
	return m_CachedLocalTransformationMatrix;
}
const Math::CMatrix4 ISceneNode::GetWorldTransformation() const
{
	return m_CachedWorldTransformationMatrix;
}
// -- Basic
Math::TVector3F ISceneNode::GetPosition() const
{
	return m_Position;
}
Math::TVector3F ISceneNode::GetScale() const
{
	return m_Scale;
}
Math::CQuaternion ISceneNode::GetOrientation() const
{
	return m_Orientation;
}
// * Setters
// -- Basic
void ISceneNode::SetPosition(Math::TVector3F position)
{
	m_Position = position;
	NeedTransformationLocaleUpdate();
}
void ISceneNode::SetOrientation(Math::CQuaternion orientation)
{
	m_Orientation = orientation;
	NeedTransformationLocaleUpdate();
}
void ISceneNode::SetScale(Math::TVector3F scale)
{
	m_Scale = scale;
	NeedTransformationLocaleUpdate();
}
void ISceneNode::LoadLocalTransformMatrix(const Math::CMatrix4& matrix)
{
	m_CachedLocalTransformationMatrix = matrix;
	// FIXME: Update all parameters
}
// -- Others
void ISceneNode::Move(Math::TVector3F offsetPosition)
{
	SetPosition(GetPosition()+offsetPosition);
}
void ISceneNode::Rotate(Math::CQuaternion rotation)
{
	SetOrientation(GetOrientation()+rotation);
}
void ISceneNode::Scale(Math::TVector3F offsetScale)
{
	SetScale(GetScale()+offsetScale);
}
// Cache system
void ISceneNode::NeedTransformationLocaleUpdate()
{
	if(!m_NeedTransformationLocalUpdate)
	{
		m_NeedTransformationLocalUpdate = true;
		NeedUpdate();
	}
}
void ISceneNode::NeedUpdate()
{
	m_NeedUpdate = true;
	if(m_Parent)
		m_Parent->NeedUpdate();
}
/*
 * Render methods
 */
void ISceneNode::UpdateTransformations()
{
	// Matrix cache update
	if(m_NeedUpdate)
	{
		m_NeedUpdate = false;

		// If need update local transformation matrix
		if(m_NeedTransformationLocalUpdate)
		{
			Math::CMatrix4 positionMatrix, scaleMatrix;
			positionMatrix.SetTranslation(m_Position.x, m_Position.y, m_Position.z);
			scaleMatrix.SetScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			m_CachedLocalTransformationMatrix = scaleMatrix*m_Orientation.ToMatrix()*positionMatrix;
			m_NeedTransformationLocalUpdate = false;
		}

		// Update world transformation matrix
		if(m_Parent)
		{
			m_CachedWorldTransformationMatrix = m_CachedLocalTransformationMatrix*m_Parent->GetWorldTransformation();
		}
		else
		{
			m_CachedWorldTransformationMatrix = m_CachedLocalTransformationMatrix;
		}
	}
}

