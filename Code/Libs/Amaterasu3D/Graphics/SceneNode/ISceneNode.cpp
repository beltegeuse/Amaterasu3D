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

#include "ISceneNode.h"
#include <Logger/Logger.h>
#include <Graphics/SceneManager.h>

namespace ama3D
{

ISceneNode::ISceneNode(const std::string& name, ISceneNode* parent) :
		m_Name(name), m_Parent(parent), m_NeedUpdate(false), m_Position( glm::vec3(0, 0, 0)),
		m_Scale( glm::vec3(1, 1, 1))
{
	if (m_Parent)
		m_Parent->AddChild(this);
}
ISceneNode::~ISceneNode()
{
	// Detach form the scenegraph
	if (m_Parent)
		m_Parent->DetachChild(this);

	// Delete all childs
	//TODO: Voir la destruction du graphe de scene
	SceneNodeList::iterator it = m_Children.begin();
	while (it != m_Children.end())
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
	if (node->GetParent())
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
	for (SceneNodeList::iterator it = m_Children.begin();
			it != m_Children.end(); ++it)
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
const glm::mat4x4 ISceneNode::GetLocalTransformation() const
{
	return m_CachedLocalTransformationMatrix;
}
const glm::mat4x4 ISceneNode::GetWorldTransformation() const
{
	return m_CachedWorldTransformationMatrix;
}
// -- Basic
 glm::vec3 ISceneNode::GetPosition() const
{
	return m_Position;
}
 glm::vec3 ISceneNode::GetScale() const
{
	return m_Scale;
}
 glm::quat ISceneNode::GetOrientation() const
{
	return m_Orientation;
}
// * Setters
// -- Basic
void ISceneNode::SetPosition( glm::vec3 position)
{
	m_Position = position;
	NeedUpdate();
}
void ISceneNode::SetOrientation(glm::quat orientation)
{
	m_Orientation = orientation;
	NeedUpdate();
}
void ISceneNode::SetScale( glm::vec3 scale)
{
	m_Scale = scale;
	NeedUpdate();
}
void ISceneNode::LoadLocalTransformMatrix(const glm::mat4x4& matrix)
{
	m_CachedLocalTransformationMatrix = matrix;
	UpdateAttributes();
	NeedUpdate();
}
// -- Others
void ISceneNode::Move( glm::vec3 offsetPosition)
{
	SetPosition(GetPosition() + offsetPosition);
}
void ISceneNode::Rotate(glm::quat rotation)
{
	SetOrientation(GetOrientation() + rotation);
}
void ISceneNode::Scale( glm::vec3 offsetScale)
{
	SetScale(GetScale() + offsetScale);
}
// Cache system
void ISceneNode::NeedUpdate()
{
	if (!m_NeedUpdate)
	{
		m_NeedUpdate = true;
		CSceneManager::Instance().NeedUpdateNextRender(this);
	}
}

/*
 * Update methods
 */
void ISceneNode::UpdateTransformations()
{
	// Matrix cache update
	if (m_NeedUpdate)
	{
		m_NeedUpdate = false;

		Logger::Log() << "[DEBUG] NeedUpdate : " << this << "\n";
		glm::mat4x4 positionMatrix, scaleMatrix;
		positionMatrix.SetTranslation(m_Position.x, m_Position.y,
				m_Position.z);
		scaleMatrix.SetScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		m_CachedLocalTransformationMatrix = scaleMatrix
				* m_Orientation.ToMatrix() * positionMatrix;
	}

	if (m_Parent)
	{
		m_CachedWorldTransformationMatrix =
				m_CachedLocalTransformationMatrix
						* m_Parent->GetWorldTransformation();
	}
	else
	{
		m_CachedWorldTransformationMatrix =
				m_CachedLocalTransformationMatrix;
	}

	// Propagate Update
	for (SceneNodeList::iterator it = m_Children.begin();
			it != m_Children.end(); it++)
		it->second->UpdateTransformations();
}

void ISceneNode::SetParent(ISceneNode* node)
{
	m_Parent = node;
	NeedUpdate(); // < To update childs
}

void ISceneNode::UpdateAttributes()
{
	m_Position = m_CachedLocalTransformationMatrix.GetTranslation();
	m_Orientation.FromMatrix(m_CachedLocalTransformationMatrix);
	m_Scale = m_CachedLocalTransformationMatrix.GetScale();
}

} // Namespace Ama3D

