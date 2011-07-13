#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Graphics/Camera/FixedCamera.h>
#include <Logger/LoggerFile.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Math/SphericalCoordinates.h>
#include <Addons/PerformancePanel/PerformancePanel.h>

#include <Application.h>
#include <Graphics/Lighting/LightingStructures.h>
#include <Graphics/Lighting/LightShaders.h>
#include <Addons/FPS/FPS.h>

#include <Addons/LPV/LPV.h>
#include <Addons/ToneMappingOperator/AbsrtactToneOperator.h>
#include <Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperator.h>
#include <Addons/ToneMappingOperator/PhotographicToneOperator/PhotographicToneOperatorExp.h>
#include <Addons/ToneMappingOperator/SumantToneOperator/SumantToneOperator.h>
#include <Addons/ToneMappingOperator/DurandToneOperator/DurandToneOperator.h>

using namespace ama3D;

class ApplicationLPV : public Application
{
protected:
	float m_AngleLight;
	// Shaders
	TShaderPtr m_GBufferShader;
	TShaderPtr m_DeferredSpotShader;
	TShaderPtr m_LPVShowVPL;
	TShaderPtr m_LPVLightingAllShader;
	TShaderPtr m_ShowLum;
	TShaderPtr m_GreyWorld;

	ManualMipmapping m_ManualMipmapping;

	AbsrtactToneOperator* m_ToneOperator;
	// LPV object
	LPV m_LPV;

	RenderableObject* m_SamplePointRSM;
	RenderableObject* m_SamplePointCamera;
	// Camera
	CameraAbstract* m_Camera;
	Texture* m_GridTexture;
	PerformancePanel m_Performances;
	// FPS Counter
	FPS m_FPS;
	// Light
	LightShaders m_Light;
	// Debug
	bool m_Debug;
	bool m_DebugGBuffer;
	bool m_DebugCompositing;
	bool m_DebugInjection;
	bool m_ShowGrid;
	bool m_DebugShowDirectOnly;
	bool m_TriInterpolation;
	bool m_DoOcclusion;
	bool m_ShowAll;
	// Grid params
	int m_PropagatedShow;
	int m_GridShow;
	int m_Level;
public:

	ApplicationLPV() :
		m_LPV(32,2.0),
		m_ManualMipmapping(128)
	{
		/////////////////
		// OpenGL setups
		/////////////////
		glPointSize(1.0);
		/////////////////
		// Models
		/////////////////
		m_SamplePointRSM = 0;
		m_SamplePointCamera = 0;
		/////////////////
		// Other variables
		/////////////////
		m_AngleLight = 70.0;
		/////////////////
		// Interactive values
		/////////////////
		m_Debug = false;
		m_DebugGBuffer = false;
		m_DebugCompositing = false;
		m_DebugInjection = false;
		m_ShowGrid = true;
		m_DebugShowDirectOnly = false;
		m_TriInterpolation = false;
		m_DoOcclusion = true;
		m_ShowAll = true;
		m_PropagatedShow = -1;
		m_GridShow = 0;
		m_Level = 0;
	}

	std::string ShowInfoCamera()
	{
		std::stringstream ss;
		ss << "Pos : " << m_Camera->GetPosition() << "\n";
		ss << "Target : " << m_Camera->GetTarget();
		return ss.str();
	}

	void UpdateLightPosition()
	{
		m_Light.Position = m_Camera->GetPosition();
		m_Light.Direction = m_Camera->GetTarget();
		//m_Light.Direction.Normalize();
	}

	void SetLightValue(float v)
	{
		m_Light.LightIntensity = v;
	}

	void LightView()
	{
		m_Camera->SetPosition(m_Light.Position);
		m_Camera->SetTarget(m_Light.Direction);
	}

private:
	/*
	 * Virtual functions
	 */
	//! Update all scene objects
	virtual void OnUpdate(double deltaTime)
	{
	}

	virtual void OnEvent(SDL_Event& event){
		if(event.type == SDL_KEYDOWN)
		{
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_Debug = !m_Debug;
					 break;
				 case SDLK_F2:
					 m_DebugGBuffer = !m_DebugGBuffer;
					 break;
				 case SDLK_F3:
					 m_DebugCompositing = !m_DebugCompositing;
					 break;
				 case SDLK_F4:
					 m_DebugInjection = !m_DebugInjection;
					 break;
				 case SDLK_F5:
					 m_ShowGrid = !m_ShowGrid;
					 break;
				 case SDLK_F6:
					 m_DebugShowDirectOnly = !m_DebugShowDirectOnly;
					 break;
				 case SDLK_F7:
					 m_TriInterpolation = !m_TriInterpolation;
					 break;
				 case SDLK_F8:
					 m_DoOcclusion = !m_DoOcclusion;
					 break;
				 case SDLK_F9:
					 m_ShowAll = !m_ShowAll; break;
				 case SDLK_q:
					 m_AngleLight = std::max(0.0, m_AngleLight - 0.1); break;
				 case SDLK_e:
					 m_AngleLight = std::min(180.0, m_AngleLight + 0.1); break;
				 case SDLK_g:
					 m_GridShow = (m_GridShow + 1) % m_LPV.GetNumberCascade(); break;
				 case SDLK_l:
					 m_Level = (m_Level + 1) %  m_LPV.GetNumberCascade(); break;
			 }
		}
	}

	//! Make all initializations
	virtual void OnInitialize()
	{
		m_LPV.Initialize();
		m_Light.Initialize();
		// Camera Setup
		// 12 -1 7   -4 .2 1.5
		m_Camera = new CameraFPS(Math::TVector3F(-12,7,1), Math::TVector3F(4,1.5,-0.2));
		//m_Camera->SetSpeed(10.0);
		// Initialise OpenGL
		glClearColor(0.0f,0.0f,0.0f,1.f);
		//////////////////
		// Load shader
		//////////////////
		// Lighting shaders
		m_LPVLightingAllShader = ShaderManager.LoadShader("LPVLightingAll.shader");
		// Others shaders
		m_GBufferShader = ShaderManager.LoadShader("GBuffer.shader");
		m_DeferredSpotShader = ShaderManager.LoadShader("DeferredSunLight.shader");
		m_LPVShowVPL = ShaderManager.LoadShader("LPVShowVPL.shader");
		m_ShowLum = ShaderManager.LoadShader("ShowLuminance.shader");

		m_GreyWorld = ShaderManager.LoadShader("GrayWorld.shader");
		m_ManualMipmapping.Initialize();
		////////////////
		// Creation of Tone mapping
		////////////////
		m_ToneOperator = new PhotographicToneOperator();
		//m_ToneOperator = new PhotographicToneOperatorExp();
		//m_ToneOperator = new SumantToneOperator();
		//m_ToneOperator = new DurandToneOperator();

		// Load Scene
		LoadSponzaScene();
		// Console commands
		Console.RegisterCommand("camera",Console::Bind(&ApplicationLPV::ShowInfoCamera, *this));
		Console.RegisterCommand("updatelight",Console::Bind(&ApplicationLPV::UpdateLightPosition, *this));
		Console.RegisterCommand("lightview",Console::Bind(&ApplicationLPV::LightView, *this));
		Console.RegisterCommand("lightvalue",Console::Bind(&ApplicationLPV::SetLightValue, *this));
		// Create sample point texture
		CreateSampleModel(512,512,&m_SamplePointRSM);
		CreateSampleModel(800,600,&m_SamplePointCamera);
	}
	//scene04No.3DS

	void LoadScene04Test()
	{
		// Create light
		//m_Light.LightColor = Color(41230,34110,29860);
		m_Light.LightColor = Color(412,341,298);
		m_Light.Position = Math::TVector3F(-12,7,1);
		m_Light.LightRaduis = 30.0;
		m_Light.LightIntensity = 1.0;
		m_Light.LightCutOff = 70;
		//m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(4,1.5,-0.2);
		IMeshSceneNode* node = SceneManager.LoadMesh("scene04No.3DS");
		node->SetScale(Math::TVector3F(0.1,0.1,0.1));
		//transMatrix.SetScaling(0.1,0.1,0.1);
		//node->LoadTransformMatrix(transMatrix);
		SceneManager.AddScenegraphRoot(node);
	}

	void LoadSponzaTest()
	{
		// Create light
		//m_Light.LightColor = Color(41230,34110,29860);
		m_Light.LightColor = Color(412,341,298);
		m_Light.Position = Math::TVector3F(-12,7,1);
		m_Light.LightRaduis = 30.0;
		m_Light.LightIntensity = 1.0;
		m_Light.LightCutOff = 70;
		//m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(4,1.5,-0.2);
		IMeshSceneNode* node = SceneManager.LoadMesh("sponza.3DS");
		//node->SetScale(Math::TVector3F(0.1,0.1,0.1));
		//node->LoadTransformMatrix(transMatrix);
		SceneManager.AddScenegraphRoot(node);
	}

	void LoadSponzaScene()
	{
		// Create light
		//m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		//m_Light.LightColor = Color(412,341,298);

		m_Light.LightColor = Color(341100,341100,341100);
		//m_Light.LightColor = Color(412300,341100,298600);

		//m_Light.Position = Math::TVector3F(-50,20,1.0);
		m_Light.Position = Math::TVector3F(-14.1498,21.6912,0.27393);

		m_Light.LightRaduis = 50.0;
		m_Light.LightIntensity = 1.0;
		//m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(-13.4795,20.9599,0.226356) - m_Light.Position;
		IMeshSceneNode* node = SceneManager.LoadMesh("sponza.obj");
		node->SetScale(Math::TVector3F(0.01,0.01,0.01));
		SceneManager.AddScenegraphRoot(node);
	}

//	void LoadTestScene()
//	{
//		// Create light
//		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
//		m_Light.Position = Math::TVector3F(80,150,0);
//		m_Light.LightRaduis = 300.0;
//		m_Light.LightIntensity = 5.0;
//		//m_Light.LightCutOff = 70;
//		m_Light.Direction = Math::TVector3F(0.0,-200.0,-400);
//		// Load scene
//		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
//		Math::CMatrix4 transMatrix;
//		transMatrix.SetScaling(0.1,0.1,0.1);
//		node->LoadTransformMatrix(transMatrix);
//		RootSceneGraph.AddChild(node);
//	}

	void CreateSampleModel(int resX, int resY, RenderableObject** model)
	{
		float * vertexBuffer = new float[resX*resY*2];
		unsigned int * indiceBuffer = new unsigned int[resX*resY];
		int l = 0;
		for(int i = 0; i < resX; i++)
			for(int j = 0; j < resY; j++)
			{
				vertexBuffer[l] =  i/(float)resX;
				vertexBuffer[l+1] =  j/(float)resX;
				l += 2;
			}
		for(int k=0; k < resX*resY; k++)
		{
			indiceBuffer[k] = k;
		}
		RenderableObject::RenderableBuffer buffer;
		buffer.buffer = vertexBuffer;
		buffer.size = resX*resY*2;
		buffer.dimension = 2;
		buffer.owner = true;
		(*model) = new RenderableObject;
		(*model)->SetDrawMode(GL_POINTS);
		(*model)->SetIndiceBuffer(indiceBuffer, resX*resY);
		(*model)->AddBuffer(buffer, VERTEX_ATTRIBUT);
		(*model)->CompileBuffers();
	}

	void DrawGrid(float resX, float resY, float offset = 0.0)
	{
		glBegin(GL_POINTS);
		for(int i = 0; i < resX; i++)
			for(int j = 0; j < resY; j++)
			{
				double k = i/(resX+offset);
				double l = j/(resY+offset);
				//glColor3ub(255,0,0);
				glVertex2d(k,l);
			}
		glColor3ub(255,255,255);
		glEnd();
	}

	void UpdateLightWorldPos()
	{
		m_Light.Position.z = 400.0*cos(m_AngleLight*(M_PI / 180.0));
		m_Light.Position.y = 400.0*sin(m_AngleLight*(M_PI / 180.0));
		m_Light.Position.x = 0.0;

		m_Light.Direction = Math::TVector3F(0,0,0);
	}

	void WhiteBalancing()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_ManualMipmapping.Compute(m_ToneOperator->GetLDRTexture());

		m_GreyWorld->Begin();
		m_ToneOperator->GetLDRTexture()->activateMultiTex(CUSTOM_TEXTURE+0);
		m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->activateMultiTex(CUSTOM_TEXTURE+1);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_ToneOperator->GetLDRTexture()->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_ManualMipmapping.GetLevel(m_ManualMipmapping.NumberLevels()-1)->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_GreyWorld->End();
	}

	//! Draw the scene
	virtual void OnRender()
	{
		//UpdateLightWorldPos();
		m_LPV.ComputeGridPosition(m_Camera);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/*
		 * 3D Drawing
		 */
		MatrixManager.SetModeMatrix(MATRIX_3D);
		m_Performances.BeginPerformanceCounter();

		// =========== First STEPS (GBuffer generation)
		// Fill in the GBuffer
		m_Performances.BeginStep("G-Buffer");
		m_GBufferShader->Begin();
		//TODO: Re-enable the gird
		if(m_ShowGrid)
		{
			m_LPV.DrawGrid(m_GridShow);
		}
//		transGrid = transGrid.Inverse();
		m_Camera->GetView();
		SceneManager.RenderAll();
		m_GBufferShader->End();
		m_Performances.EndStep();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ========== Second STEPS (RSM generation buffers)
		m_Performances.BeginStep("RSM");
		m_Light.Compute();
		m_Performances.EndStep();

		// ============= Compute Indirect lighting only
		glClearColor(0.0f,0.0f,0.0f,0.f);

		// ******* 2nd Step : Geometry injection
		m_Performances.BeginStep("  === Injection Geometry");
		m_LPV.BeginInjectionGeometryPass(m_Level);
		if(m_DoOcclusion)
		{
			m_LPV.InjectGeometryFromLight(m_Light, *m_SamplePointRSM);
		}
		m_LPV.EndInjectionGeometryPass();
		m_Performances.EndStep();
		// ****** 1st Step : VPL Injection
		m_Performances.BeginStep("  === Injection VPL");
		m_LPV.BeginInjectionVPLPass(m_Level);
		m_LPV.InjectVPLFromLight(m_Light, *m_SamplePointRSM);
		m_LPV.EndInjectionVPLPass();
		m_Performances.EndStep();
		// ******* 3th Step : Diffusion
		m_Performances.BeginStep("  === Propagation");
		m_LPV.ComputePropagation(16);
		m_Performances.EndStep();
		// ******* 4th Step : Filtrage pass
		// WARNING : Don't forgot to add uniform
		m_Performances.BeginStep("Lighting");
		if(m_ShowAll)
		{
//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			m_LPVLightingAllShader->Begin();
			// Pass LPV Grid
			m_LPV.GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_LPV.GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_LPV.GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
			// Pass Deferred lighting
			m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+4);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+5);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+6);
			m_Light.GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+7); // For Shadow Mapping
			// Pass Uniform LPV
			m_LPV.SetGridInformations(m_LPVLightingAllShader);
			// Pass Uniform Deferred
			m_LPVLightingAllShader->SetUniform1f("LightRaduis",m_Light.LightRaduis);
			//m_LPVLightingAllShader->SetUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
			m_LPVLightingAllShader->SetUniform1f("LightIntensity", m_Light.LightIntensity);
			m_LPVLightingAllShader->SetUniformVector("LightPosition", m_Light.Position);
			m_LPVLightingAllShader->SetUniformVector("LightSpotDirection", m_Light.Direction- m_Light.Position);
			m_LPVLightingAllShader->SetUniformColor("LightColor", m_Light.LightColor);
			// * Shadow Map propreties
			m_LPVLightingAllShader->SetUniformMatrix4fv("LightViewMatrix", m_Light.GetViewMatrix());
			m_LPVLightingAllShader->SetUniformMatrix4fv("LightProjectionMatrix", m_Light.GetProjectionMatrix());
			// Pass Uniform to know the position
			ShaderHelperUniformPositionFromView(m_LPVLightingAllShader);
			// Draw ...
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2f(-1.0, -1.0);
				glTexCoord2f(0.0, 1.0);
				glVertex2f(-1.0, 1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex2f(1.0, 1.0);
				glTexCoord2f(1.0, 0.0);
				glVertex2f(1.0, -1.0);
			glEnd();
			// desactivate LPV Grid
			m_LPV.GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_LPV.GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_LPV.GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			// desactivate Deferred lighting
			m_GBufferShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+4);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+5);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+6);
			m_Light.GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+7); // For Shadow Mapping

			m_LPVLightingAllShader->End();

			m_LPVLightingAllShader->GetFBO()->DrawDebug();

//			m_ToneOperator->Begin();
//			m_LPVLightingAllShader->GetFBO()->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+0);
//			glBegin(GL_QUADS);
//				glTexCoord2f(0.0, 0.0);
//				glVertex2f(-1.0, -1.0);
//				glTexCoord2f(0.0, 1.0);
//				glVertex2f(-1.0, 1.0);
//				glTexCoord2f(1.0, 1.0);
//				glVertex2f(1.0, 1.0);
//				glTexCoord2f(1.0, 0.0);
//				glVertex2f(1.0, -1.0);
//			glEnd();
//			m_LPVLightingAllShader->GetFBO()->GetTexture("Color")->desactivateMultiTex(CUSTOM_TEXTURE+0);
//			m_ToneOperator->End();

			m_ShowLum->Begin();
			m_LPVLightingAllShader->GetFBO()->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+0);
			//m_Light.GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2f(-1.0, -1.0);
				glTexCoord2f(0.0, 1.0);
				glVertex2f(-1.0, 1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex2f(1.0, 1.0);
				glTexCoord2f(1.0, 0.0);
				glVertex2f(1.0, -1.0);
			glEnd();
			m_LPVLightingAllShader->GetFBO()->GetTexture("Color")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_ShowLum->End();
			m_ShowLum->GetFBO()->DrawDebug();
//			m_ToneOperator->Compress(m_LPVLightingAllShader->GetFBO()->GetTexture("Color"));
//
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//			m_ToneOperator->DrawDebug();
//
//			WhiteBalancing();

			//m_LPV.m_LPVInjectGeomerty->GetFBO()->DrawDebug();
		}
		else
		{
			m_LPV.ShowDebugPropagation(m_GBufferShader, m_PropagatedShow);
			glClearColor(0.0f,0.0f,0.0f,1.f);
			// ============= Compute Direct lighting only
			if(m_DebugShowDirectOnly)
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_Light.GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+4); // For Shadow Mapping
			m_DeferredSpotShader->Begin();
			// Go to spot pass
			// * Light propreties
			m_DeferredSpotShader->SetUniform1i("DebugMode", m_Debug);
			m_DeferredSpotShader->SetUniform1f("LightRaduis",m_Light.LightRaduis);
			m_DeferredSpotShader->SetUniform1f("LightCutOff", cos(90.0 *(M_PI / 180.0)));
			m_DeferredSpotShader->SetUniform1f("LightIntensity", m_Light.LightIntensity);
			m_DeferredSpotShader->SetUniformVector("LightPosition", m_Light.Position);
			m_DeferredSpotShader->SetUniformVector("LightSpotDirection", m_Light.Direction- m_Light.Position);
			m_DeferredSpotShader->SetUniformColor("LightColor", m_Light.LightColor);
			// * Shadow Map propreties
			m_DeferredSpotShader->SetUniformMatrix4fv("LightViewMatrix", m_Light.GetViewMatrix());
			m_DeferredSpotShader->SetUniformMatrix4fv("LightProjectionMatrix", m_Light.GetProjectionMatrix());
			// * Position propreties
			ShaderHelperUniformPositionFromView(m_DeferredSpotShader);
			// Draw ...
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0);
				glVertex2f(-1.0, -1.0);
				glTexCoord2f(0.0, 1.0);
				glVertex2f(-1.0, 1.0);
				glTexCoord2f(1.0, 1.0);
				glVertex2f(1.0, 1.0);
				glTexCoord2f(1.0, 0.0);
				glVertex2f(1.0, -1.0);
			glEnd();
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			m_GBufferShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_Light.GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+4);
			m_DeferredSpotShader->End();
		}

		if(m_DebugCompositing)
		{
			m_Light.GetFBO()->DrawDebug();
		}
		if(m_DebugGBuffer)
		{
			m_GBufferShader->GetFBO()->DrawDebug();
		}
		if(m_DebugInjection)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Logger::Log() << "Draw Injection ... \n";
			m_LPVShowVPL->Begin();
			m_Light.GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_Light.GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_Light.GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+3);
			ShaderHelperUniformPosition(m_LPVShowVPL, m_Light.GetProjectionMatrix(), m_Light.GetViewMatrix(), 1.0, m_Light.LightRaduis);
			m_SamplePointRSM->Draw();
			m_Light.GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_Light.GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			m_Light.GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVShowVPL->End();
		}
		m_Performances.EndStep();
		m_Performances.EndPerformanceCounter();
		/*
		 * 2D Drawing
		 */
		MatrixManager.SetModeMatrix(MATRIX_2D);
		Console.Draw();
		m_Performances.Draw();

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
	// FIXME: Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");

	std::cout << "[INFO] Begin ..." << std::endl;
	ApplicationLPV application;
	application.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
