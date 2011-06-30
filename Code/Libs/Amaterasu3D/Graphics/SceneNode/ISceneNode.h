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

#ifndef ISCENENODE_H_
#define ISCENENODE_H_

// Amaterasu3D includes
#include <Math/Matrix4.h>
#include <Math/Quaternion.h>

// STL includes
#include <map>
#include <vector>
#include <string>

class ISceneNode {
protected:
	/*
	 * Typedef
	 */
	typedef std::map<std::string, ISceneNode*> SceneNodeList;

	/*
	 * Attributes
	 */
	// Hierachie
	SceneNodeList m_Children;

	// Node attributes
	std::string m_Name;

	// Heirachical attributes
	ISceneNode * m_Parent;

	// Geometrical attributes
	// * Cache matrix management
	bool m_NeedUpdate;
	bool m_NeedTransformationLocalUpdate;
	Math::CMatrix4 m_CachedWorldTransformationMatrix;
	Math::CMatrix4 m_CachedLocalTransformationMatrix;
	// * Geometry information
	Math::TVector3F m_Position;
	Math::CQuaternion m_Orientation;
	Math::TVector3F m_Scale;


public:
	ISceneNode(const std::string& name, ISceneNode* parent);
	virtual ~ISceneNode();

	// General methods
	const std::string& GetName() const;

	// Hierachical management
	// * Global information
	ISceneNode* GetParent();

	// * Change hierachie
	void AddChild(ISceneNode* node);
	void DetachChild(ISceneNode* node);
	void DeleteAllChildren();

	// Transformation management
	bool IsNeedUpdate() const;
	// * Getters
	// -- Transformation
	const Math::CMatrix4 GetLocalTransformation() const;
	const Math::CMatrix4 GetWorldTransformation() const;
	// -- Basic
	Math::TVector3F GetPosition() const;
	Math::TVector3F GetScale() const;
	Math::CQuaternion GetOrientation() const;
	// * Setters
	// -- Basic
	void SetPosition(Math::TVector3F position);
	void SetOrientation(Math::CQuaternion orientation);
	void SetScale(Math::TVector3F scale);
	void LoadLocalTransformMatrix(const Math::CMatrix4& matrix);
	// -- Others
	void Move(Math::TVector3F offsetPosition);
	void Rotate(Math::CQuaternion rotation);
	void Scale(Math::TVector3F offsetScale);

	// Culling information
	//TODO: Do this part (Bounding box, sphere, ... etc).

	// Virtual pure methods
	virtual void UpdateTransformations();

protected:
	//! Info : Can be call by parent or Transformation setters
	void NeedTransformationLocaleUpdate(); // < For matrix locale cache only
	void NeedUpdate(); // < For know you or parents need Transformation update
	void SetParent(ISceneNode* node);
};

#endif /* ISCENENODE_H_ */
