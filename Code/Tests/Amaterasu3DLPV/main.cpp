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
	SceneGraph::Model* m_GridModel;
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
	// Grid params
	Math::TVector3F m_CellSize;
	Math::TVector3F m_GirdPosition;
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

	virtual ~ApplicationLPV()
	{
		delete m_GridModel;
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
			 }
		}
	}
	//! Make all initializations
	virtual void OnInitialize()
	{
		// Setup variables
		m_Debug = false;
		m_DebugGBuffer = false;
		m_DebugCompositing = false;
		m_DebugInjection = false;
		m_ShowGrid = true;
		m_DebugShowDirectOnly = false;
		glPointSize(10.0);
		m_CellSize = Math::TVector3F(10.0,10.0,10.0);
		m_GirdPosition = Math::TVector3F(-100.0,-100.0,-200.0);
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
		// Create Grid
//		CreateGridTexture();
		CreateGridModel();
	}

	void CreateGridModel()
	{
		// Allocation des buffers
		float * vertexBuffer = new float[3*32*32*3*2];
		float * colorBuffer = new float[3*32*32*3*2];
		unsigned int* indiceBuffer = new unsigned int[3*32*32*2];
		int i = 0;
		int m_CellCount = 32;
		Color color(0.0,0.0,1.0);
		// Fill in buffers
		for(int z=0;z<=32;z++){
			for(int x=0;x<=32;x++){
//				Logger::Log() << i << '\n';
				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = 0+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = m_CellCount*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}

			for(int y=0;y<=32;y++){
				vertexBuffer[i] = 0+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = m_CellCount*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = z*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}
		}

		for(int y=0;y<=32;y++){
			for(int x=0;x<32;x++){
				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = 0+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;

				vertexBuffer[i] = x*m_CellSize.x+m_GirdPosition.x;
				vertexBuffer[i+1] = y*m_CellSize.y+m_GirdPosition.y;
				vertexBuffer[i+2] = m_CellCount*m_CellSize.z+m_GirdPosition.z;
				colorBuffer[i] = color.R;
				colorBuffer[i+1] = color.G;
				colorBuffer[i+2] = color.B;
				i += 3;
			}
		}

		for(int l=0; l < 3*32*32*2; l++)
		{
			indiceBuffer[l] = l;
		}

		m_GridModel = new SceneGraph::Model;
		m_GridModel->SetDrawMode(GL_LINES);
		m_GridModel->SetIndiceBuffer(indiceBuffer, 3*32*32*2);
		SceneGraph::ModelBuffer buffer;
		buffer.buffer = vertexBuffer;
		buffer.size = 3*32*32*3*2;
		buffer.dimension = 3;
		buffer.owner = true;
		m_GridModel->AddBuffer(buffer, VERTEX_ATTRIBUT);
		buffer.buffer = colorBuffer;
		m_GridModel->AddBuffer(buffer, COLOR_ATTRIBUT);
		m_GridModel->CompileBuffers();
		m_GridModel->AddMaterial(DIFFUSE_MATERIAL,color);
	}

	void CreateGridTexture()
	{
		m_GridTexture = new Texture(true,0,GL_TEXTURE_3D);
		m_GridTexture->activateTexture();
		Texture3DParams param;
		param.applyParam();
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, 32, 32, 32, 0, GL_RGBA, GL_FLOAT, 0);
	}

	void DrawGrid(float resX, float resY)
	{
		glBegin(GL_POINTS);
		for(int i = 0; i < resX; i++)
			for(int j = 0; j < resY; j++)
			{
				double k = i/resX;
				double l = j/resY;
				glColor3ub(255,0,0);
				glVertex2d(k,l);
			}
		glColor3ub(255,255,255);
		glEnd();
	}

	//! Draw the scene
	virtual void OnRender()
	{
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
		// ****** 1st Step : VPL Injection
		m_LPVInjectVPL->begin();
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->setUniform3f("GridPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
		DrawGrid(512.0,512.0);
		m_RSMSpotShader->GetFBO()->GetTexture("Flux")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_RSMSpotShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_RSMSpotShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVInjectVPL->end();

		// ******* 2nd Step : Geometry injection

		// ******* 3th Step : Diffusion

		// ******* 4th Step : Filtrage pass
		// WARNING : Don't forgot to add uniform
		m_LPVLightingShader->begin();
		m_LPVInjectVPL->GetFBO()->GetTexture("Grid")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVLightingShader->setUniform3f("GridPosition", m_GirdPosition.x,m_GirdPosition.y,m_GirdPosition.z);
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
		m_LPVInjectVPL->GetFBO()->GetTexture("Grid")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_GBufferShader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_GBufferShader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_LPVLightingShader->end();

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
			m_LPVInjectVPL->GetFBO()->GetTexture("Grid")->activateMultiTex(CUSTOM_TEXTURE+0);
			DrawGrid(256.0,128.0);
			m_LPVInjectVPL->GetFBO()->GetTexture("Grid")->desactivateMultiTex(CUSTOM_TEXTURE+0);
			m_LPVShowVPL->end();
		}

		/*
		 * 2D Drawing
		 */
		MatrixManager.SetModeMatrix(MATRIX_2D);
//		glMatrixMode(GL_PROJECTION);
//		glPushMatrix();
//		glLoadMatrixf(MatrixManager.GetMatrix(PROJECTION_MATRIX));
//		DrawGrid(800.0/2.0,600.0/2.0);
//		glPopMatrix();
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
