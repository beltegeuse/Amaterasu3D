/*
 * ICameraSceneNode.cpp
 *
 *  Created on: Jun 9, 2011
 *      Author: adrien
 */

#include "ICameraSceneNode.h"

ICameraSceneNode::ICameraSceneNode(const std::string& name, ISceneNode* parent) :
	ISceneNode(name, parent)
{
}

ICameraSceneNode::~ICameraSceneNode()
{
}

// Projection management
// * General
const Math::CMatrix4 ICameraSceneNode::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}
// * Setters
void ICameraSceneNode::SetFOV(float fov)
{
	m_FOV = fov;
}
void ICameraSceneNode::SetRatio(float ratio)
{
	m_Ratio = ratio;
}
void ICameraSceneNode::SetNear(float near)
{
	m_Near = near;
}
void ICameraSceneNode::SetFar(float far)
{
	m_Far = far;
}
// * Getters
float ICameraSceneNode::GetFOV() const
{
	return m_FOV;
}
float ICameraSceneNode::GetRatio() const
{
	return m_Ratio;
}
float ICameraSceneNode::GetNear() const
{
	return m_Near;
}
float ICameraSceneNode::GetFar() const
{
	return m_Far;
}
void ICameraSceneNode::UpdateTransformations()
{
	ISceneNode::UpdateTransformations();
	if(m_NeedUpdateProjectionMatrix)
	{
		m_ProjectionMatrix = Math::CMatrix4::CreatePerspectiveFOV(m_FOV, m_Ratio, m_Near, m_Far);
		m_NeedUpdateProjectionMatrix = false;
	}
}
void ICameraSceneNode::NeedProjectionUpdate()
{
	if(!m_NeedUpdateProjectionMatrix)
	{
		m_NeedUpdateProjectionMatrix = true;
		NeedUpdate();
	}
}
