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
#include "glm/glm.hpp"
#include "glm/ext.hpp"

// STL includes
#include <map>
#include <vector>
#include <string>

namespace ama3D
{
class ISceneNode
{
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
	glm::mat4x4 m_CachedWorldTransformationMatrix;
	glm::mat4x4 m_CachedLocalTransformationMatrix;
	// * Geometry information
	 glm::vec3 m_Position;
	 glm::fquat m_Orientation;
	 glm::vec3 m_Scale;

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
	const glm::mat4x4 GetLocalTransformation() const;
	const glm::mat4x4 GetWorldTransformation() const;
	// -- Basic
	 glm::vec3 GetPosition() const;
	 glm::vec3 GetScale() const;
	 glm::fquat GetOrientation() const;
	// * Setters
	// -- Basic
	void SetPosition( glm::vec3 position);
	void SetOrientation(glm::fquat orientation);
	void SetScale( glm::vec3 scale);
	void LoadLocalTransformMatrix(const glm::mat4x4& matrix);
	// -- Others
	void Move( glm::vec3 offsetPosition);
	void Rotate(glm::fquat rotation);
	void Scale( glm::vec3 offsetScale);

	// Culling information
	//TODO: Do this part (Bounding box, sphere, ... etc).

	// Virtual pure methods
	virtual void UpdateTransformations();

protected:
	//! Info : Can be call by parent or Transformation setters
	void NeedUpdate(); // < For matrix locale cache only
	void SetParent(ISceneNode* node);
	void UpdateAttributes(); // < Update attributes form the matrix
};
}

#endif /* ISCENENODE_H_ */
