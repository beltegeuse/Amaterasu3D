/*
 * ICameraSceneNode.h
 *
 *  Created on: Jun 9, 2011
 *      Author: adrien
 */

#ifndef ICAMERASCENENODE_H_
#define ICAMERASCENENODE_H_

// Amaterasu3D includes
#include <Graphics/SceneNode/ISceneNode.h>
#include <Math/Matrix4.h>

class ICameraSceneNode : public ISceneNode
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
	ICameraSceneNode();
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

	void Render() {}
};

#endif /* ICAMERASCENENODE_H_ */
