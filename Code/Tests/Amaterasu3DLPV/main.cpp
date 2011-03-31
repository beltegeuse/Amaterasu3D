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

#include <Application.h>
#include <Graphics/Lighting/LightingStructures.h>
#include <Addons/FPS/FPS.h>
class ApplicationLPV : public Application
{
protected:
	// Shaders
	TShaderPtr m_GBufferShader;
	TShaderPtr m_RSMSpotShader;
	TShaderPtr m_DeferredSpotShader;
	TShaderPtr m_LPVInjectVPL;
	TShaderPtr m_LPVShowVPL;
	TShaderPtr m_BasicShader;
	TShaderPtr m_LPVLightingShader;
	TShaderPtr m_LPVPropagationShader;
	TShaderPtr m_LPVInjectGeomerty;
	TShaderPtr m_LPVBlend;
	FBO** m_PropagationFBOs;
	SceneGraph::Model* m_GridModel;
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
			 }
		}
	}
	//! Make all initializations
	virtual void OnInitialize()
	{
		m_GridModel = 0;
		m_SamplePointRSM = 0;
		m_SamplePointCamera = 0;

		// Setup variables
		m_Debug = false;
		m_DebugGBuffer = false;
		m_DebugCompositing = false;
		m_DebugInjection = false;
		m_ShowGrid = true;
		m_DebugShowDirectOnly = false;
		m_TriInterpolation = false;
		m_DoOcclusion = false;
		m_PropagatedShow = -1;
		glPointSize(1.0);
		m_CellSize = Math::TVector3F(10.0,10.0,10.0);
		m_GirdPosition = Math::TVector3F(-100.0,-100.0,-200.0);
		m_NbCellDim = 32;
		m_TextureSize = Math::TVector2I(256,128);
		m_NbPropagationStep = 16;
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(6,102,72), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Initialise OpenGL
		glClearColor(0.0f,0.0f,0.0f,1.f);
		MatrixManager.SetProjectionMatrix(Math::CMatrix4::CreatePerspectiveFOV(70, (double)800/600, 1.0, 400));
		// Load shader
		m_GBufferShader = ShaderManager.LoadShader("GBuffer.shader");
		m_RSMSpotShader = ShaderManager.LoadShader("RefectiveShadowMapSpot.shader");
		m_DeferredSpotShader = ShaderManager.LoadShader("DeferredSpotLight.shader");
		m_LPVInjectVPL = ShaderManager.LoadShader("LPVInjectVPL.shader");
		m_LPVShowVPL = ShaderManager.LoadShader("LPVShowVPL.shader");
		m_BasicShader = ShaderManager.LoadShader("BasicShader.shader");
		m_LPVLightingShader = ShaderManager.LoadShader("LPVLighting.shader");
		m_LPVInjectGeomerty = ShaderManager.LoadShader("LPVInjectGeometry.shader");
		m_LPVPropagationShader = ShaderManager.LoadShader("LPVPropagation.shader");
		m_LPVBlend = ShaderManager.LoadShader("LPVBlend.shader");

		// Copy buffer
		m_PropagationFBOs = new FBO*[m_NbPropagationStep];
		m_PropagationFBOs[0] = m_LPVPropagationShader->GetFBO();
		for(int i = 1; i < m_NbPropagationStep; i++)
		{
			Logger::Log() << "[INFO] Create Copy Propagation FBO : " << i << "\n";
			m_PropagationFBOs[i] = m_LPVPropagationShader->GetFBO()->Copy();
		}
		// Create light
		m_Light.LightColor = Color(1.0,1.0,1.0,0.0);
		m_Light.Position = Math::TVector3F(80,125,60);
		m_Light.LightRaduis = 300.0;
		m_Light.LightIntensity = 10.0;
		m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(0.0,0.0,-400);
//		m_Light.Direction.Normalize();
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("TestScene2.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);
		// Console commands
		Console.RegisterCommand("camera",Console::Bind(&ApplicationLPV::ShowInfoCamera, *this));
		Console.RegisterCommand("updatelight",Console::Bind(&ApplicationLPV::UpdateLightPosition, *this));
		Console.RegisterCommand("lightview",Console::Bind(&ApplicationLPV::LightView, *this));
		// Create Grid
		CreateGridModel();
		// Create sample point texture
		CreateSampleModel(512,512,&m_SamplePointRSM);
		CreateSampleModel(800,600,&m_SamplePointCamera);
	}

	void CreateGridModel()
	{
		// Allocation des buffers
		float * vertexBuffer = new float[3*m_NbCellDim*m_NbCellDim*3*2];
		float * colorBuffer = new float[3*m_NbCellDim*m_NbCellDim*3*2];
		unsigned int* indiceBuffer = new unsigned int[3*m_NbCellDim*m_NbCellDim*2];
		int i = 0;
		Color color(0.0,0.0,1.0);
		// Fill in buffers
		for(int z=0;z<=m_NbCellDim;z++){
			for(int x=0;x<=m_NbCellDim;x++){
//				Logger::Log() << i << '\n';
				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = 0+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = m_NbCellDim*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}

			for(int y=0;y<=m_NbCellDim;y++){
				vertexBuffer[i] = 0+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = m_NbCellDim*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}
		}

		for(int y=0;y<=m_NbCellDim;y++){
			for(int x=0;x<m_NbCellDim;x++){
				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = 0+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = m_NbCellDim*m_CellSize.z+m_GirdPosition.z;
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

		m_GridModel = new SceneGraph::Model;
		m_GridModel->SetDrawMode(GL_LINES);
		m_GridModel->SetIndiceBuffer(indiceBuffer, 3*m_NbCellDim*m_NbCellDim*2);
		SceneGraph::ModelBuffer buffer;
		buffer.buffer = vertexBuffer;
		buffer.size = 3*m_NbCellDim*m_NbCellDim*3*2;
		buffer.dimension = 3;
		buffer.owner = true;
		m_GridModel->AddBuffer(buffer, VERTEX_ATTRIBUT);
		buffer.buffer = colorBuffer;
		m_GridModel->AddBuffer(buffer, COLOR_ATTRIBUT);
		m_GridModel->CompileBuffers();
		m_GridModel->AddMaterial(DIFFUSE_MATERIAL,color);
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

	//! Draw the scene
	virtual void OnRender()
	{
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
		m_GBufferShader->begin();
		if(m_ShowGrid)
		{
//			m_BasicShader->begin();
			m_GridModel->Draw();
//			m_BasicShader->end();
		}
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_GBufferShader->end();

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
		m_RSMSpotShader->begin();
		// *** Send all Uniform values
		m_RSMSpotShader->setUniform1f("LightRaduis",m_Light.LightRaduis);
		m_RSMSpotShader->setUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
		m_RSMSpotShader->setUniform1f("LightIntensity", m_Light.LightIntensity);
		m_RSMSpotShader->setUniform3f("LightPosition", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
		m_RSMSpotShader->setUniform3f("LightSpotDirection", m_Light.Direction.x, m_Light.Direction.y, m_Light.Direction.z);
		m_RSMSpotShader->setUniform3f("LightColor", m_Light.LightColor.R, m_Light.LightColor.G, m_Light.LightColor.B);
		// * Draw the scene
		RootSceneGraph.Draw();
		m_RSMSpotShader->end();
		// * Revert transformations
		MatrixManager.SetProjectionMatrix(oldProjectionMatrix);
		MatrixManager.SetViewMatrix(oldViewMatrix);

		// ============= Compute Indirect lighting only
		glClearColor(0.0f,0.0f,0.0f,0.f);
		// ****** 1st Step : VPL Injection
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE);
		m_LPVInjectVPL->begin();
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->setUniform3f("LPVPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
		m_LPVInjectVPL->setUniform4f("LPVSize",m_TextureSize.x,m_TextureSize.y,8.0,4.0);
		m_LPVInjectVPL->setUniform4f("LPVCellSize",m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim);
		m_SamplePointRSM->Draw();
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->end();
		glDisable(GL_BLEND);

		// ******* 2nd Step : Geometry injection
		if(m_DoOcclusion)
		{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE,GL_ONE);
		m_LPVInjectGeomerty->begin();
		m_LPVInjectGeomerty->setUniform3f("LPVPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
		m_LPVInjectGeomerty->setUniform4f("LPVSize",m_TextureSize.x,m_TextureSize.y,8.0,4.0);
		m_LPVInjectGeomerty->setUniform4f("LPVCellSize",m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim);
		// ------- From Lights
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_SamplePointRSM->Draw();
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		// ------- From Camera
//		m_GBufferShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+0);
//		m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
//		m_GBufferShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+2);
//		m_SamplePointCamera->Draw();
//		m_GBufferShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+0);
//		m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
//		m_GBufferShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		// --------- Restore all states
		m_LPVInjectGeomerty->end();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		}
		// ******* 3th Step : Diffusion
		for(int i = 0; i < m_NbPropagationStep; i++)
		{
			//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			//Logger::Log() << "Propagation : " << i << "\n";
			m_LPVPropagationShader->SetFBO(m_PropagationFBOs[i], false);
			m_LPVPropagationShader->begin();
			//m_LPVPropagationShader->setUniform3f("LPVPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
			m_LPVPropagationShader->setUniform4f("LPVSize",m_TextureSize.x,m_TextureSize.y,8.0,4.0);
			m_LPVPropagationShader->setUniform4f("LPVCellSize",m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim);
			m_LPVInjectGeomerty->GetFBO()->GetTexture("Grid")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVPropagationShader->setUniform1i("DoOcclusion",m_DoOcclusion);
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
			m_LPVPropagationShader->end();
		}

		// ******* 3th bis Step : Blend all propagations
		m_LPVBlend->begin();
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE,GL_ONE);
		for(int i = 0; i < m_PropagatedShow+1; i++)
		{
//			if(i == 0)
//			{
//				m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
//				m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
//				m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
//			}
//			else
//			{
				m_PropagationFBOs[i]->GetTexture("GridRed")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_PropagationFBOs[i]->GetTexture("GridGreen")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_PropagationFBOs[i]->GetTexture("GridBlue")->activateMultiTex(CUSTOM_TEXTURE+2);
//			}
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
//			if(i == 0)
//			{
//				m_LPVInjectVPL->GetFBO()->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
//				m_LPVInjectVPL->GetFBO()->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
//				m_LPVInjectVPL->GetFBO()->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
//			}
//			else
//			{
				m_PropagationFBOs[i]->GetTexture("GridRed")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_PropagationFBOs[i]->GetTexture("GridGreen")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_PropagationFBOs[i]->GetTexture("GridBlue")->desactivateMultiTex(CUSTOM_TEXTURE+2);
//			}
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_LPVBlend->end();

		// ******* 4th Step : Filtrage pass
		// WARNING : Don't forgot to add uniform
		m_LPVLightingShader->begin();
		m_GBufferShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+1);
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
		m_LPVLightingShader->setUniform3f("LPVPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
		m_LPVLightingShader->setUniform4f("LPVSize",m_TextureSize.x,m_TextureSize.y,8.0,4.0);
		m_LPVLightingShader->setUniform4f("LPVCellSize",m_CellSize.x,m_CellSize.y,m_CellSize.z,m_NbCellDim);
		m_LPVLightingShader->setUniform1i("EnableTrilinearInterpolation",m_TriInterpolation);
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
		m_GBufferShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_LPVLightingShader->end();

		glClearColor(0.0f,0.0f,0.0f,1.f);
		// ============= Compute Direct lighting only
		if(m_DebugShowDirectOnly)
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_GBufferShader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Specular")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_GBufferShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+3);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+4);
		m_DeferredSpotShader->begin();
		// Go to spot pass
		// * Light propreties
		m_DeferredSpotShader->setUniform1i("DebugMode", m_Debug);
		m_DeferredSpotShader->setUniform1f("LightRaduis",m_Light.LightRaduis);
		m_DeferredSpotShader->setUniform1f("LightCutOff", cos(m_Light.LightCutOff *(M_PI / 180.0)));
		m_DeferredSpotShader->setUniform1f("LightIntensity", m_Light.LightIntensity);
		m_DeferredSpotShader->setUniform3f("LightPosition", m_Light.Position.x, m_Light.Position.y, m_Light.Position.z);
		m_DeferredSpotShader->setUniform3f("LightSpotDirection", m_Light.Direction.x, m_Light.Direction.y, m_Light.Direction.z);
		m_DeferredSpotShader->setUniform3f("LightColor", m_Light.LightColor.R, m_Light.LightColor.G, m_Light.LightColor.B);
		// * Shadow Map propreties
		m_DeferredSpotShader->setUniformMatrix4fv("LightViewMatrix", LightViewMatrix);
		m_DeferredSpotShader->setUniformMatrix4fv("LightProjectionMatrix", LightProjectionMatrix);
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
		m_GBufferShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+3);
		m_RSMSpotShader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+4);
		m_DeferredSpotShader->end();

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
			m_LPVShowVPL->begin();
			m_RSMSpotShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+1);
			m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
			m_RSMSpotShader->GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+3);
			m_SamplePointRSM->Draw();
			m_RSMSpotShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+1);
			m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
			m_RSMSpotShader->GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+3);
			m_LPVShowVPL->end();
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
