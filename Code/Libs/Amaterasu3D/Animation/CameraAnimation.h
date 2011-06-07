/*
 * CameraAnimation.h
 *
 *  Created on: Jun 7, 2011
 *      Author: adrien
 */

#ifndef CAMERAANIMATION_H_
#define CAMERAANIMATION_H_

struct CameraAnimationControlPoint
{
	/*
	 * Attributes
	 */
	Math::TVector3F Position;
	Math::TVector3F Direction;

	/*
	 * Constructors & Destructors
	 */
	CameraAnimationControlPoint(const Math::TVector3F& position, const Math::TVector3F& direction) :
		Position(position), Direction(direction)
	{
		Direction.Normalize();
	}
};

class CameraAnimation
{
protected:
	/*
	 * Attributes
	 */
	std::vector<CameraAnimationControlPoint> m_Points;
	std::vector<float> m_TimePoints;

	CatmullRomInterpolator<Math::TVector3F> m_PositionInterpolator;
	CatmullRomInterpolator<float> m_TimeInterpolator;

	CameraAbstract* m_Camera;
	bool m_IsCompiled;
	float m_TotalTime;
	bool m_LoopAnimation;
	int m_AnimationSequence;
public:
	/*
	 * Constructor & Destructor
	 */
	CameraAnimation(CameraAbstract* camera) : m_Camera(camera), m_IsCompiled(false), m_TotalTime(0.0), m_LoopAnimation(true)
	{}
	virtual ~CameraAnimation() {}

	CameraAbstract* GetCamera() { return m_Camera; }

	// Add Control point and delta time with other control point
	void AddControlPoint(const CameraAnimationControlPoint& p, float deltaTime)
	{
		m_Points.push_back(p);
		if(m_TimePoints.empty())
			m_TimePoints.push_back(0.0);
		else
			m_TimePoints.push_back(deltaTime+(*m_TimePoints.rbegin()));
	}

	void EraseAllControlPoints()
	{
		m_Points.erase(m_Points.begin(), m_Points.end());
		m_TimePoints.erase(m_TimePoints.begin(), m_TimePoints.end());
	}

	// Initialise the animation & Initialise all interpolation system
	// need to be call when control point is add to take into account
	void Compile()
	{
		Assert(m_Points.size() > 1);

		// say all interpolators compiled & rest total time
		m_IsCompiled = true;
		m_TotalTime = 0.0;
		m_AnimationSequence = 0;
		// Reinitialise all interpolators
		m_PositionInterpolator.EraseAllPoints();
		m_TimeInterpolator.EraseAllPoints();

		// Add all points
		for(int i = 0; i < m_Points.size(); i++)
			m_PositionInterpolator.AddPoint(m_Points[i].Position);
		for(int i = 0; i < m_TimePoints.size(); i++)
			m_TimeInterpolator.AddPoint(m_TimePoints[i]);
	}

	void Update(float deltaTime)
	{
		Assert(m_IsCompiled);

		// Update time
		m_TotalTime += deltaTime;

		// If looping
		if(m_LoopAnimation && m_TotalTime > (*m_TimePoints.rbegin()))
		{
			m_TotalTime = 0.0;
			m_AnimationSequence = 1;
		}

		// Update anime sequence
		//TODO: Protected to jump
		while(m_TimePoints[m_AnimationSequence] < m_TotalTime)
			m_AnimationSequence++;

		// Position interpolation
		float deltaTinterpolation = (m_TotalTime - m_TimePoints[m_AnimationSequence-1]) / ((float)(m_TimePoints[m_AnimationSequence]-m_TimePoints[m_AnimationSequence-1]));
		Math::TVector3F newPos =  m_PositionInterpolator.Interpolation(m_AnimationSequence + deltaTinterpolation-1);

		// Direction interpolation
		Math::CQuaternion q1;
		Math::CQuaternion q2;

		q1.From3DVector(m_Points[m_AnimationSequence-1].Direction);
		q2.From3DVector(m_Points[m_AnimationSequence].Direction);

		Math::TVector3F newDir = Math::CQuaternion::slerp(q1, q2, deltaTinterpolation).ToMatrix().Transform(Math::TVector3F(1,0,0),1);


		std::cout << "New Camera position : " << newPos << std::endl;
		std::cout << "New Camera orientation : " << newDir << std::endl;
		std::cout << "Total time : " << m_TotalTime << std::endl;

		// Update camera
		m_Camera->SetPosition(newPos);
		m_Camera->SetTarget(newPos+newDir);
	}
};

#endif /* CAMERAANIMATION_H_ */
