#include "ISceneNode.h"
#include <Logger/Logger.h>
ISceneNode::ISceneNode(const std::string& name, ISceneNode* parent) : m_Name(name), m_Parent(parent),
m_NeedUpdate(false), m_NeedTransformationLocalUpdate(false), m_Position(Math::TVector3F(0,0,0)), m_Scale(Math::TVector3F(1,1,1))
{
	if(m_Parent)
		m_Parent->AddChild(this);
}
ISceneNode::~ISceneNode()
{
	// Detach form the scenegraph
	if(m_Parent)
		m_Parent->DetachChild(this);

	// Delete all childs
	//TODO: Voir la destruction du graphe de scene
	SceneNodeList::iterator it = m_Children.begin();
	while(it != m_Children.end())
	{
		delete it->second; // < With the delete => Unattach child
		it = m_Children.begin();
	}
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

	// Need detach node
	if(node->GetParent())
		node->GetParent()->DetachChild(node);

	m_Children[node->GetName()] = node;
	node->SetParent(this);
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
bool ISceneNode::IsNeedUpdate() const
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
	if(!m_NeedUpdate)
	{
		Logger::Log() << "[DEBUG] ISceneNode::NeedUpdate() \n";
		m_NeedUpdate = true;
		if(m_Parent)
			m_Parent->NeedUpdate();
	}
}
/*
 * Render methods
 */
void ISceneNode::UpdateTransformations()
{
	//Logger::Log() << "[DEBUG] ISceneNode::UpdateTransformations() \n";
	// Matrix cache update
	if(m_NeedUpdate)
	{
		m_NeedUpdate = false;

		Logger::Log() << "[DEBUG] NeedUpdate : " << this << "\n";
		// If need update local transformation matrix
		if(m_NeedTransformationLocalUpdate)
		{
			Logger::Log() << "[DEBUG] NeedLocalUpdate : " << this << "\n";
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

		// Propagate Update
		for(SceneNodeList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
			it->second->UpdateTransformations();
	}
}

void ISceneNode::SetParent(ISceneNode* node)
{
	m_Parent = node;
	// TODO: Miss call in changes
	if(IsNeedUpdate())
	{
		m_NeedUpdate = false; // < Force to renotify
		NeedUpdate(); // < Notify to parent
	}
}

