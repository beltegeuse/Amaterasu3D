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

#ifndef ICAMERASCENENODE_H_
#define ICAMERASCENENODE_H_

// Amaterasu3D includes
#include <Graphics/SceneNode/ISceneNode.h>
#include <Math/Matrix4.h>

class ICameraSceneNode: public ISceneNode
{
private:
	/*
	 * Attributes
	 */
	// View matrix
	Math::CMatrix4 m_ViewMatrix;
	// Projection attributes
	// * Cache projection matrix
	Math::CMatrix4 m_ProjectionMatrix;
	bool m_NeedUpdateProjectionMatrix;
	// * Projection attributes
	float m_Near;
	float m_Far;
	float m_FOV;
	float m_Ratio;

	//TODO: See Abstract camera for other things ...

public:
	ICameraSceneNode(const std::string& name, ISceneNode* parent);
	virtual ~ICameraSceneNode();

	// View management
	const Math::CMatrix4 GetViewMatrix();

	// Projection management
	// * General
	const Math::CMatrix4 GetProjectionMatrix();
	// * Setters
	void SetFOV(float fov);
	void SetRatio(float ratio);
	void SetNear(float near);
	void SetFar(float far);
	// * Getters
	float GetFOV() const;
	float GetRatio() const;
	float GetNear() const;
	float GetFar() const;

	// To update Camera
	virtual void UpdateTransformations();
private:
	void NeedProjectionUpdate();
};

#endif /* ICAMERASCENENODE_H_ */
