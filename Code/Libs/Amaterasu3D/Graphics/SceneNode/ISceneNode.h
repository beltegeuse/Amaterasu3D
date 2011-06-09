/*
 * ISceneNode.h
 *
 *  Created on: Jun 9, 2011
 *      Author: adrien
 */

#ifndef ISCENENODE_H_
#define ISCENENODE_H_

// Amaterasu3D includes
#include <Math/Matrix4.h>

// STL includes
#include <map>
#include <vector>
#include <string>

class ISceneNode {
protected:
	/*
	 * Typedef
	 */
	std::map<std::string, ISceneNode*> m_Children;

	/*
	 * Attributes
	 */
	// Node attributes
	std::string m_Name;

	// Heirachical attributes
	ISceneNode * m_Parent;

	// Geometrical attributes
	// * Cache matrix management
	bool m_NeedTransformationUpdate;
	Math::CMatrix4 m_CachedWorldTransformationMatrix;
	Math::CMatrix4 m_CachedLocalTransformationMatrix;
	// * Geometry information
	Math::TVector3F m_Position;
	Math::CQuaternion m_Orientation;
	Math::TVector3F m_Scale;


public:
	ISceneNode(ISceneNode* parent);
	virtual ~ISceneNode();

	// General methods
	const std::string& GetName() const;

	// Hierachical management
	// * Global information
	ISceneNode* GetParent();

	// * Change hierachie
	void AddChild(ISceneNode* node);
	void DeleteChild(ISceneNode* node);
	void DeleteAllChildren();

	// Transformation management
	bool IsNeedTransformationCacheUpdate() const;
	// * Getters
	const Math::CMatrix4 GetLocalTransformation() const;
	const Math::CMatrix4 GetWorldTransformation() const;
	// * Setters
	// -- Basic
	void SetPosition(Math::TVector3F position);
	void SetOrientation(Math::CQuaternion orientation);
	void SetScale(Math::TVector3F scale);
	// -- Others
	void Move(Math::TVector3F offsetPosition);
	void Rotate(Math::CQuaternion rotation);
	void Scale(Math::TVector3F offsetScale);

	// Culling information
	//TODO: Do this part (Bounding box, sphere, ... etc).

	// Virtual pure methods
	void Render() = 0;

protected:
	//! Info : Can be call by parent or Transformation setters
	void NeedTransformationCacheUpdate();

};

#endif /* ISCENENODE_H_ */
