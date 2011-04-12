#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Model.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Logger/LoggerFile.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Graphics/SceneGraph/Other/Skydome.h>

#include <Application.h>
#include <Graphics/Lighting/LightingStructures.h>
#include <Addons/FPS/FPS.h>
class ApplicationLPV : public Application
{
protected:
	float m_AngleLight;
	// Shaders
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	TShaderPtr m_DeferredSpotShader;
	TShaderPtr m_LPVInjectVPL;
	TShaderPtr m_LPVShowVPL;
	TShaderPtr m_BasicShader;
	TShaderPtr m_LPVLightingShader;
	TShaderPtr m_LPVLightingAllShader;
	TShaderPtr m_LPVPropagationShader;
	TShaderPtr m_LPVInjectGeomerty;
	TShaderPtr m_LPVBlend;
	FBO** m_PropagationFBOs;
	Skydome* m_Sky;
	SceneGraph::Group* m_GridModel;
	SceneGraph::Model* m_SamplePointRSM;
	SceneGraph::Model* m_SamplePointCamera;
	// Camera
	CameraFPS* m_Camera;
	Texture* m_GridTexture;
	// FPS Counter
	FPS m_FPS;
	// Light
	SpotLight m_Light;
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
	Math::TVector3F m_CellSize;
	Math::TVector3F m_GirdPosition;
	Math::TVector2I m_TextureSize;
	int m_NbCellDim;
	int m_NbPropagationStep;
	int m_PropagatedShow;
public:

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

	virtual ~ApplicationLPV()
	{
		delete m_GridModel;
		delete[] m_PropagationFBOs;
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
				 case SDLK_UP:
					 if(m_PropagatedShow < m_NbPropagationStep-1)
						 m_PropagatedShow++;
					 Logger::Log() << "[SHOW] Propagated Grid : " << m_PropagatedShow << "\n";
					 break;
				 case SDLK_DOWN:
					 if(m_PropagatedShow > -1)
						 m_PropagatedShow--;
					 Logger::Log() << "[SHOW] Propagated Grid : " << m_PropagatedShow << "\n";
					 break;
				 case SDLK_F8:
					 m_DoOcclusion = !m_DoOcclusion;
					 break;
				 case SDLK_F9:
					 m_ShowAll = !m_ShowAll; break;
				 case SDLK_8:
					 m_GirdPosition.x += 1.0;
					 break;
				 case SDLK_2:
					 m_GirdPosition.x -= 1.0; break;
				 case SDLK_4:
					 m_GirdPosition.z += 1.0; break;
				 case SDLK_6:
					 m_GirdPosition.z -= 1.0; break;
				 case SDLK_q:
					 m_AngleLight = std::max(0.0, m_AngleLight - 0.1); break;
				 case SDLK_e:
					 m_AngleLight = std::min(180.0, m_AngleLight + 0.1); break;
			 }
		}
	}
	//! Make all initializations
	virtual void OnInitialize()
	{
		m_Sky = new Skydome;
		m_GridModel = 0;
		m_SamplePointRSM = 0;
		m_SamplePointCamera = 0;
		m_AngleLight = 70.0;
		// Setup variables
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
		glPointSize(1.0);
		m_CellSize = Math::TVector3F(10.0,10.0,10.0);
		m_GirdPosition = Math::TVector3F(-98.0,-98.0,-198.0);
		m_NbCellDim = 32;
		m_TextureSize = Math::TVector2I(256,128);
		m_NbPropagationStep = 16;
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(6,102,72), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Initialise OpenGL
		glClearColor(0.0f,0.0f,0.0f,1.f);
		// Load shader
		m_GBufferShader = ShaderManager.LoadShader("GBuffer.shader");
		m_RSMSpotShader = ShaderManager.LoadShader("RefectiveShadowMapSun.shader");
		m_DeferredSpotShader = ShaderManager.LoadShader("DeferredSunLight.shader");
		m_LPVInjectVPL = ShaderManager.LoadShader("LPVInjectVPL.shader");
		m_LPVShowVPL = ShaderManager.LoadShader("LPVShowVPL.shader");
		m_BasicShader = ShaderManager.LoadShader("BasicShader.shader");
		m_LPVLightingShader = ShaderManager.LoadShader("LPVLighting.shader");
		m_LPVInjectGeomerty = ShaderManager.LoadShader("LPVInjectGeometry.shader");
		m_LPVPropagationShader = ShaderManager.LoadShader("LPVPropagation.shader");
		m_LPVBlend = ShaderManager.LoadShader("LPVBlend.shader");
		m_LPVLightingAllShader = ShaderManager.LoadShader("LPVLightingAll.shader");
		// Copy buffer
		m_PropagationFBOs = new FBO*[m_NbPropagationStep];
		m_PropagationFBOs[0] = m_LPVPropagationShader->GetFBO();
		for(int i = 1; i < m_NbPropagationStep; i++)
		{
			Logger::Log() << "[INFO] Create Copy Propagation FBO : " << i << "\n";
			m_PropagationFBOs[i] = m_LPVPropagationShader->GetFBO()->Copy();
		}
		// Load Scene
		LoadSponzaScene();
		// Console commands
		Console.RegisterCommand("camera",Console::Bind(&ApplicationLPV::ShowInfoCamera, *this));
		Console.RegisterCommand("updatelight",Console::Bind(&ApplicationLPV::UpdateLightPosition, *this));
		Console.RegisterCommand("lightview",Console::Bind(&ApplicationLPV::LightView, *this));
		Console.RegisterCommand("lightvalue",Console::Bind(&ApplicationLPV::SetLightValue, *this));
		// Create Grid
		CreateGridModel();
		// Create sample point texture
		CreateSampleModel(512,512,&m_SamplePointRSM);
		CreateSampleModel(800,600,&m_SamplePointCamera);
	}

	void LoadSponzaScene()
	{
		// Create light
		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		//m_Light.Position = Math::TVector3F(-11,171,22);
		m_Light.Position = Math::TVector3F(-50,20,1.0);
		m_Light.LightRaduis = 500.0;
		m_Light.LightIntensity = 2.0;
		m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(0.1,0.0,0.0);
		//m_Light.Direction = Math::TVector3F(0.0,0.0,1.0);
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);
	}

	void LoadTestScene()
	{
		// Create light
		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_Light.Position = Math::TVector3F(80,150,0);
		m_Light.LightRaduis = 300.0;
		m_Light.LightIntensity = 5.0;
		m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(0.0,-200.0,-400);
//		m_Light.Direction.Normalize();
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);
	}

	void CreateGridModel()
	{
		// Allocation des buffers
		float * vertexBuffer = new float[3*m_NbCellDim*m_NbCellDim*3*2];
		float * colorBuffer = new float[3*m_NbCellDim*m_NbCellDim*3*2];
		unsigned int* indiceBuffer = new unsigned int[3*m_NbCellDim*m_NbCellDim*2];
		int i = 0;
		Color color(1.0,1.0,1.0);
		// Fill in buffers
		for(int z=0;z<=m_NbCellDim;z++){
			for(int x=0;x<=m_NbCellDim;x++){
//				Logger::Log() << i << '\n';
				vertexBuffer[i] = x*m_CellSize.x;
				vertexBuffer[i+1] = 0;
				vertexBuffer[i+2] = z*m_CellSize.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = x*m_CellSize.x;
				vertexBuffer[i+1] = m_NbCellDim*m_CellSize.y;
				vertexBuffer[i+2] = z*m_CellSize.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}

			for(int y=0;y<=m_NbCellDim;y++){
				vertexBuffer[i] = 0;
				vertexBuffer[i+1] = y*m_CellSize.y;
				vertexBuffer[i+2] = z*m_CellSize.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = m_NbCellDim*m_CellSize.x;
				vertexBuffer[i+1] = y*m_CellSize.y;
				vertexBuffer[i+2] = z*m_CellSize.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}
		}

		for(int y=0;y<=m_NbCellDim;y++){
			for(int x=0;x<m_NbCellDim;x++){
				vertexBuffer[i] = x*m_CellSize.x;
				vertexBuffer[i+1] = y*m_CellSize.y;
				vertexBuffer[i+2] = 0;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = x*m_CellSize.x;
				vertexBuffer[i+1] = y*m_CellSize.y;
				vertexBuffer[i+2] = m_NbCellDim*m_CellSize.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}
		}

		for(int l=0; l < 3*m_NbCellDim*m_NbCellDim*2; l++)
		{
			indiceBuffer[l] = l;
		}

		SceneGraph::Model* model = new SceneGraph::Model;
		model->SetDrawMode(GL_LINES);
		model->SetIndiceBuffer(indiceBuffer, 3*m_NbCellDim*m_NbCellDim*2);
		SceneGraph::ModelBuffer buffer;
		buffer.buffer = vertexBuffer;
		buffer.size = 3*m_NbCellDim*m_NbCellDim*3*2;
		buffer.dimension = 3;
		buffer.owner = true;
		model->AddBuffer(buffer, VERTEX_ATTRIBUT);
		buffer.buffer = colorBuffer;
		model->AddBuffer(buffer, COLOR_ATTRIBUT);
		model->CompileBuffers();
		model->AddMaterial(DIFFUSE_MATERIAL,color);

		m_GridModel = new SceneGraph::Group;
		m_GridModel->AddChild(model);

	}

	void CreateSampleModel(int resX, int resY, SceneGraph::Model** model)
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
		SceneGraph::ModelBuffer buffer;
		buffer.buffer = vertexBuffer;
		buffer.size = resX*resY*2;
		buffer.dimension = 2;
		buffer.owner = true;
		(*model) = new SceneGraph::Model;
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

	//! Draw the scene
	virtual void OnRender()
	{
		UpdateLightWorldPos();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/*
		 * 3D Drawing
		 */
		MatrixManager.SetModeMatrix(MATRIX_3D);

		// =========== First STEPS (GBuffer generation)
		// Fill in the GBuffer
		m_GBufferShader->Begin();
		if(m_ShowGrid)
		{
			Math::CMatrix4 transGrid;
			transGrid.SetTranslation(m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
			m_GridModel->LoadTransformMatrix(transGrid);
			m_GridModel->Draw();
		}
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_GBufferShader->End();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ========== Second STEPS (RSM generation buffers)
		// Fill in RSM spot buffers
		// * Matrix Setup
		Math::CMatrix4 LightViewMatrix;
		LightViewMatrix.LookAt(m_Light.Position, m_Light.Direction);
		Math::CMatrix4 LightProjectionMatrix;
		LightProjectionMatrix = Math::CMatrix4::CreatePerspectiveFOV(m_Light.LightCutOff, 512.0/512.0, 1.0, m_Light.LightRaduis);
		Math::CMatrix4 oldProjectionMatrix;
		Math::CMatrix4 oldViewMatrix;
		// * Save old transformations
		oldProjectionMatrix = MatrixManager.GetMatrix(PROJECTION_MATRIX);
		oldViewMatrix = MatrixManager.GetMatrix(VIEW_MATRIX);
		// * Go to the camera view
		MatrixManager.SetProjectionMatrix(LightProjectionMatrix);
		MatrixManager.SetViewMatrix(LightViewMatrix);
		// * Enable Shader
		m_RSMSpotShader->Begin();
		// *** Send all Uniform values
		m_RSMSpotShader->SetUniform1f("LightRaduis",m_Light.LightRaduis);
//		m_RSMSpotShader->SetUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
//		m_RSMSpotShader->SetUniform1f("LightIntensity", m_Light.LightIntensity);
		m_RSMSpotShader->SetUniformVector("LightPosition", m_Light.Position);
//		m_RSMSpotShader->SetUniformVector("LightSpotDirection", m_Light.Direction - m_Light.Position);
		m_RSMSpotShader->SetUniformColor("LightColor", m_Light.LightColor);
		// * Draw the scene
		RootSceneGraph.Draw();
		m_RSMSpotShader->End();
		// * Revert transformations
		MatrixManager.SetProjectionMatrix(oldProjectionMatrix);
		MatrixManager.SetViewMatrix(oldViewMatrix);

		// ============= Compute Indirect lighting only
		glClearColor(0.0f,0.0f,0.0f,0.f);
		// ****** 1st Step : VPL Injection
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE);
		m_LPVInjectVPL->Begin();
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->SetUniformVector("LPVPosition", m_GirdPosition);
		m_LPVInjectVPL->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,8.0,4.0));
		m_LPVInjectVPL->SetUniformVector("LPVCellSize",Math::TVector4F(m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim));
		ShaderHelperUniformPosition(m_LPVInjectVPL, LightProjectionMatrix, LightViewMatrix, 1.0, m_Light.LightRaduis);
		m_SamplePointRSM->Draw();
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->End();
		glDisable(GL_BLEND);

		// ******* 2nd Step : Geometry injection
		if(m_DoOcclusion)
		{
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_ONE,GL_ONE);
			m_LPVInjectGeomerty->Begin();
			m_LPVInjectGeomerty->SetUniformVector("LPVPosition", m_GirdPosition);
			m_LPVInjectGeomerty->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,8.0,4.0));
			m_LPVInjectGeomerty->SetUniformVector("LPVCellSize",Math::TVector4F(m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim));
			// ------- From Lights
			m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+2);
			ShaderHelperUniformPosition(m_LPVInjectGeomerty,LightProjectionMatrix, LightViewMatrix, 1.0, m_Light.LightRaduis);
			m_SamplePointRSM->Draw();
			m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			// ------- From Camera
//			m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
//			m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+2);
//			ShaderHelperUniformPositionFromView(m_LPVInjectGeomerty);
//			m_SamplePointCamera->Draw();
//			m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
//			m_GBufferShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			// --------- Restore all states
			m_LPVInjectGeomerty->End();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		}
		else
		{
			// Just for clear buffers
			m_LPVInjectGeomerty->Begin();
			m_LPVInjectGeomerty->End();
		}
		// ******* 3th Step : Diffusion
		for(int i = 0; i < m_NbPropagationStep; i++)
		{
			//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			//Logger::Log() << "Propagation : " << i << "\n";
			m_LPVPropagationShader->SetFBO(m_PropagationFBOs[i], false);
			m_LPVPropagationShader->Begin();
			//m_LPVPropagationShader->setUniform3f("LPVPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
			m_LPVPropagationShader->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,8.0,4.0));
			m_LPVPropagationShader->SetUniformVector("LPVCellSize",Math::TVector4F(m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim));
			m_LPVInjectGeomerty->GetFBO()->GetTexture("Grid")->activateMultiTex(CUSTOM_TEXTURE+3);
			//m_LPVPropagationShader->SetUniform1i("DoOcclusion",m_DoOcclusion);
			if(i == 0)
			{
				m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
			}
			else
			{
				m_PropagationFBOs[i-1]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_PropagationFBOs[i-1]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_PropagationFBOs[i-1]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
			}
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
			if(i == 0)
			{
				m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			}
			else
			{
				m_PropagationFBOs[i-1]->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_PropagationFBOs[i-1]->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_PropagationFBOs[i-1]->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			}
			m_LPVInjectGeomerty->GetFBO()->GetTexture("Grid")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVPropagationShader->End();
		}

		// ******* 3th bis Step : Blend all propagations
		m_LPVBlend->Begin();
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE,GL_ONE);
		for(int i = 0; i < m_PropagatedShow+2; i++)
		{
			if(i == 0)
			{
				m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
			}
			else
			{
				m_PropagationFBOs[i-1]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_PropagationFBOs[i-1]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_PropagationFBOs[i-1]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
			}
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
			if(i == 0)
			{
				m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			}
			else
			{
				m_PropagationFBOs[i-1]->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_PropagationFBOs[i-1]->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_PropagationFBOs[i-1]->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			}
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_LPVBlend->End();

		// ******* 4th Step : Filtrage pass
		// WARNING : Don't forgot to add uniform
		if(m_ShowAll)
		{
//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//			m_Sky->Draw();
			m_LPVLightingAllShader->Begin();
			// Pass LPV Grid
			m_LPVBlend->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVBlend->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_LPVBlend->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
			// Pass Deferred lighting
			m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+4);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+5);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+6);
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+7); // For Shadow Mapping
			// Pass Uniform LPV
			m_LPVLightingAllShader->SetUniformVector("LPVPosition", m_GirdPosition);
			m_LPVLightingAllShader->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,8.0,4.0));
			m_LPVLightingAllShader->SetUniformVector("LPVCellSize",Math::TVector4F(m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim));
			// Pass Uniform Deferred
			m_LPVLightingAllShader->SetUniform1f("LightRaduis",m_Light.LightRaduis);
			//m_LPVLightingAllShader->SetUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
			m_LPVLightingAllShader->SetUniform1f("LightIntensity", m_Light.LightIntensity);
			m_LPVLightingAllShader->SetUniformVector("LightPosition", m_Light.Position);
			m_LPVLightingAllShader->SetUniformVector("LightSpotDirection", m_Light.Direction- m_Light.Position);
			m_LPVLightingAllShader->SetUniformColor("LightColor", m_Light.LightColor);
			// * Shadow Map propreties
			m_LPVLightingAllShader->SetUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
			m_LPVLightingAllShader->SetUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
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
			m_LPVBlend->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVBlend->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_LPVBlend->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			// desactivate Deferred lighting
			m_GBufferShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+4);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+5);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->desactivateMultiTex(CUSTOM_TEXTURE+6);
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+7); // For Shadow Mapping

			m_LPVLightingAllShader->End();

//			glDisable(GL_BLEND);
		}
		else
		{
			// ******* 4th Step : Filtrage pass
			// WARNING : Don't forgot to add uniform
			m_LPVLightingShader->Begin();
			m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+5);
			if(m_PropagatedShow < 0)
			{
			m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+4);
			}
			else
			{
				m_PropagationFBOs[m_PropagatedShow]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+2);
				m_PropagationFBOs[m_PropagatedShow]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+3);
				m_PropagationFBOs[m_PropagatedShow]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+4);
			}
			m_LPVBlend->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_LPVBlend->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVBlend->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+4);
			m_LPVLightingShader->SetUniformVector("LPVPosition", m_GirdPosition);
			m_LPVLightingShader->SetUniformVector("LPVSize",Math::TVector4F(m_TextureSize.x,m_TextureSize.y,8.0,4.0));
			m_LPVLightingShader->SetUniformVector("LPVCellSize",Math::TVector4F(m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim));
			m_LPVLightingShader->SetUniform1i("EnableTrilinearInterpolation",m_TriInterpolation);
			ShaderHelperUniformPositionFromView(m_LPVLightingShader);
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
			//FIXME: Same pattern ???
			m_LPVBlend->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			m_LPVBlend->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVBlend->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+4);
			m_GBufferShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+5);
			m_LPVLightingShader->End();

			glClearColor(0.0f,0.0f,0.0f,1.f);
			// ============= Compute Direct lighting only

			if(m_DebugShowDirectOnly)
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
			m_GBufferShader->GetFBO()->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+4); // For Shadow Mapping
			m_DeferredSpotShader->Begin();
			// Go to spot pass
			// * Light propreties
			m_DeferredSpotShader->SetUniform1i("DebugMode", m_Debug);
			m_DeferredSpotShader->SetUniform1f("LightRaduis",m_Light.LightRaduis);
			m_DeferredSpotShader->SetUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
			m_DeferredSpotShader->SetUniform1f("LightIntensity", m_Light.LightIntensity);
			m_DeferredSpotShader->SetUniformVector("LightPosition", m_Light.Position);
			m_DeferredSpotShader->SetUniformVector("LightSpotDirection", m_Light.Direction- m_Light.Position);
			m_DeferredSpotShader->SetUniformColor("LightColor", m_Light.LightColor);
			// * Shadow Map propreties
			m_DeferredSpotShader->SetUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
			m_DeferredSpotShader->SetUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
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
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+4);
			m_DeferredSpotShader->End();
		}

		if(m_DebugCompositing)
		{
			m_RSMSpotShader->GetFBO()->DrawDebug();
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
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_RSMSpotShader->GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+3);
			ShaderHelperUniformPosition(m_LPVShowVPL, LightProjectionMatrix, LightViewMatrix, 1.0, m_Light.LightRaduis);
			m_SamplePointRSM->Draw();
			m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			m_RSMSpotShader->GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVShowVPL->End();
		}

		/*
		 * 2D Drawing
		 */
		MatrixManager.SetModeMatrix(MATRIX_2D);
		Console.Draw();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
