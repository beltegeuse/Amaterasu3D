#ifndef CAMERAABSTRACT_H_
#define CAMERAABSTRACT_H_

#include <Math/Vector3.h>
#include <Math/Matrix4.h>
#include <SDL/SDL_events.h>

class CameraAbstract
{
protected:
	// Attributs
	Math::CMatrix4 m_matrix;
	// * Pour le GluLookAt
	Math::TVector3F m_position;
	Math::TVector3F m_target;
	Math::TVector3F m_up;
public:
	// Constructor & destructors
	CameraAbstract(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up = Math::TVector3F(0,0,1));
	virtual ~CameraAbstract();

	void SetMatrix(const Math::CMatrix4& matrix);
	virtual void ComputeMatrix();
	const Math::CMatrix4& GetMatrix();
	virtual void OnEvent(SDL_Event& events, double deltaTime) = 0;
};

#endif /* CAMERAABSTRACT_H_ */
