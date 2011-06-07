#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Model.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Graphics/Camera/FixedCamera.h>
#include <Addons/FPS/FPS.h>

#include <Math/Quaternion.h>
#include <Math/Vector3.h>
template <class T> class CatmullRomInterpolation
{
protected:
	/*
	 * Attributes
	 */
	std::vector<T> m_Points;

	/*
	 * Private methods
	 */
	T ComputeInterpolation(T& p0, T& p1, T& p2, T& p3, float t)
	{
		return ((2.f*p1) + (p2 + (-p0))*t + (p0*2.f + p1*(-5.f) + p2*4.f + p3*(-1.f))*t*t + (p0*(-1.f) + p1*3.f + (-3.f)*p2 + p3)*t*t*t)*0.5f;
	}
public:
	/*
	 * Constructor & Destructors
	 */
	CatmullRomInterpolation() {}
	virtual ~CatmullRomInterpolation() {}

	/*
	 * Points managements
	 */
	void AddPoint(const T& v)
	{
		if(m_Points.empty())
		{
			m_Points.push_back(v);
			m_Points.push_back(v);
		}
		m_Points[m_Points.size() - 1] = v;
		m_Points.push_back(v);
	}
	void EraseAllPoints() { m_Points.erase(m_Points.begin(), m_Points.end()); }

	T Interpolation(float t)
	{
		Assert(t >= 0.0 && (t+1) <= (m_Points.size()+2) && m_Points.size() > 4);

		int id = floor(t);

		return ComputeInterpolation(m_Points[id], m_Points[id+1], m_Points[id+2], m_Points[id+3], t - id);
	}
};

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

	CatmullRomInterpolation<Math::TVector3F> m_PositionInterpolator;
	CatmullRomInterpolation<float> m_TimeInterpolator;

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

class ApplicationCamera : public Application
{
protected:
	CameraFPS* m_Camera;
	CameraAnimation* m_CameraAnimation;
	TShaderPtr m_gbuffer_shader;
	DeferredLighting* m_GI;
	FPS m_FPS;
	bool m_debug;
	bool m_play;
public:
	/*
	 * Console methods
	 */
	std::string ConsoleAddControlPoint(float deltaTime)
	{
		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(m_Camera->GetPosition(), m_Camera->GetTarget() - m_Camera->GetPosition()), deltaTime);
		std::stringstream ss;
		ss << "Add point : " << m_Camera->GetPosition() << " | " << m_Camera->GetTarget() - m_Camera->GetPosition() << " ( " << deltaTime << ")";
		return ss.str();
	}

	void ConsoleCompilePlay()
	{
		m_play = true;
		m_CameraAnimation->Compile();
	}

	void ConsoleStop()
	{
		m_play = false;
	}

	void ConsoleEraseAll()
	{
		m_CameraAnimation->EraseAllControlPoints();
	}

	/*
	 * Constructor and destructors
	 */
	ApplicationCamera() :
		m_debug(false),
		m_play(false)
	{

	}

	virtual ~ApplicationCamera()
	{
	}

	/*
	 * Other methods
	 */
	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(200.0);
		m_CameraAnimation = new CameraAnimation(new FixedCamera(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0)));
		AddControlPoints();
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(1.0,4000.0,70.0);
		// Load shader
		m_gbuffer_shader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		// Load GI
		m_GI = new DeferredLighting(RootSceneGraph);
		m_GI->SetFBOGraphicBuffer(m_gbuffer_shader->GetFBO());
		// Create light 2
		SpotLight light2;
		light2.LightColor = Color(1.0,1.0,1.0,0.0);
		light2.Position = Math::TVector3F(-500,200,0);
		light2.LightRaduis = 4000.0;
		light2.LightIntensity = 1.0;
		light2.LightCutOff = 70;
		light2.Direction = Math::TVector3F(1.0,0.0,0.0);
		m_GI->AddSpotLight(light2);
		// Load scene
		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
		RootSceneGraph.AddChild(node1);

		// Add Console functions
		Console.RegisterCommand("add",Console::Bind(&ApplicationCamera::ConsoleAddControlPoint, *this));
		Console.RegisterCommand("compile",Console::Bind(&ApplicationCamera::ConsoleCompilePlay, *this));
		Console.RegisterCommand("stop",Console::Bind(&ApplicationCamera::ConsoleStop, *this));
		Console.RegisterCommand("erase",Console::Bind(&ApplicationCamera::ConsoleEraseAll, *this));
	}

	void AddControlPoints()
	{
//		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,1,0)),10);
//		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,-1,0)),10);
		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(1,0,0)),0);
		m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(-1,0,0)),10);
		//m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,0,1)),10);
		//m_CameraAnimation->AddControlPoint(CameraAnimationControlPoint(Math::TVector3F(3,4,2), Math::TVector3F(0,0,-1)),10);

	}

	virtual void OnUpdate(double delta)
	{
		if(m_play)
			m_CameraAnimation->Update(delta);
	}

	virtual void OnEvent(SDL_Event& event)
	{
		if(event.type == SDL_KEYDOWN)
		{
			Math::CMatrix4 matrixTransform;
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_debug = !m_debug;
					 break;
				 case SDLK_F2:
					 m_GI->SetDebugMode(!m_GI->isDebugMode());
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_gbuffer_shader->Begin();

		if(!m_play)
			m_Camera->GetView();
		else
			m_CameraAnimation->GetCamera()->GetView();

		RootSceneGraph.Draw();
		m_gbuffer_shader->End();

		if(m_debug)
			m_gbuffer_shader->GetFBO()->DrawDebug();
		else
		{
			if(!m_play)
				m_Camera->GetView();
			else
				m_CameraAnimation->GetCamera()->GetView();

			m_GI->ComputeIllumination();
		}

		MatrixManager.SetModeMatrix(MATRIX_2D);

		Console.Draw();
	}
};

#ifdef WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;

	ApplicationCamera window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
