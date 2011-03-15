#ifndef CAMERAFLY_H_
#define CAMERAFLY_H_

#include <Graphics/Camera/CameraAbstract.h>
#include <Math/Vector3.h>

class CameraFly : public CameraAbstract
{
private:
	// Pour bouger...
	Math::TVector3F m_forward;
	Math::TVector3F m_left;

	float m_angleX;
	float m_angleY;

	// Les valeurs temporaires
	bool m_mouse_click;
	float m_angleX_new;
	float m_angleY_new;

	// Les constantes
	float m_factor_move;

	// For keyboard
	bool m_up_press;
	bool m_down_press;
	bool m_left_press;
	bool m_right_press;
public:
	CameraFly(const Math::TVector3F& pos, const Math::TVector3F& target, const Math::TVector3F& up = Math::TVector3F(0,1,0));
	virtual ~CameraFly();

	virtual void OnEvent(SDL_Event& event, double deltaTime);
	virtual void ComputeMatrix(double delta);
	void SetSpeed(float moveSec);

private:
	void ComputeAngles();
};

#endif /* CAMERAFLY_H_ */
