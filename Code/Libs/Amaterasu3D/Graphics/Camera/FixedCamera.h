/*
 * FixedCamera.h
 *
 *  Created on: May 27, 2011
 *      Author: adrien
 */

#ifndef FIXEDCAMERA_H_
#define FIXEDCAMERA_H_

#include <Graphics/Camera/CameraAbstract.h>
#include <Math/Vector3.h>

class FixedCamera : public CameraAbstract
{
public:
	FixedCamera(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up = Math::TVector3F(0,1,0));
	virtual ~FixedCamera();

	virtual void MousePressed() {}
	virtual void MouseReleased() {}
	virtual void KeyPressed(SDL_Keycode& key) {}
	virtual void KeyReleased(SDL_Keycode& key) {}
	virtual void MouseMoved(int x, int y) {}
	virtual void FrameStarted(double delta) {}
	virtual void FrameEnded() {}
};

#endif /* FIXEDCAMERA_H_ */
