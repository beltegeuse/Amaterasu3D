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
#include <Addons/FPS/FPS.h>
#include <Graphics/SceneGraph/Rectangle2D.h>
#include <Addons/LPV/LPV.h>
#include <Graphics/Lighting/LightShaders.h>

struct FBOCube
{
	FBO* Fbo;
	Math::CMatrix4 ProjectionMatrix;
	Math::CMatrix4 TransMatrix;
	Rectangle2D* Model;
};

class ApplicationWhite : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_GreyWorld;
	TShaderPtr m_CreateMipmapping;
	TShaderPtr m_TestMipmapping;

	TShaderPtr m_GBufferShader;

	TShaderPtr m_DeferredSpotShader;
	TShaderPtr m_LPVLightingAllShader;
	LightShaders m_Light;
	float m_AngleLight;

	SceneGraph::Model* m_SamplePointRSM;
	SceneGraph::Model* m_SamplePointCamera;

	LPV m_LPV;

	FPS m_FPS;
	FBOCube* m_CubeFBOs;
	bool m_debug;
	bool m_ParaboloidDraw;
	bool m_HemiDraw;
public:
	ApplicationWhite() :
		m_debug(false),
		m_ParaboloidDraw(false),
		m_HemiDraw(false)
	{
		m_AngleLight = 70.0;
	}

	virtual ~ApplicationWhite()
	{
	}

	virtual void OnInitialize()
	{
		m_LPV.Initialize();
		m_Light.Initialize();
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(200.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		SettingsManager.SetProjection(1.0,4000.0,70.0);
		// Load shader
		m_GreyWorld = CShaderManager::Instance().LoadShader("GrayWorld.shader");
		m_CreateMipmapping = CShaderManager::Instance().LoadShader("CreateMipmapping.shader");
		m_GBufferShader = ShaderManager.LoadShader("GBuffer.shader");
		m_DeferredSpotShader = ShaderManager.LoadShader("DeferredSunLight.shader");
		m_TestMipmapping = CShaderManager::Instance().LoadShader("TestMipmappingRead.shader");
		m_LPVLightingAllShader = ShaderManager.LoadShader("LPVLightingAll.shader");
		// Creation des autres buffers
//		GenerateCubeBuffers();
		// Load scene
//		SceneGraph::AssimpNode* node1 = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
//		RootSceneGraph.AddChild(node1);

		m_Light.LightColor = Color(0.8,0.5,0.1,1.0);
		m_Light.Position = Math::TVector3F(-50,20,1.0);
		m_Light.LightRaduis = 500.0;
		m_Light.LightIntensity = 1.0;
		//m_Light.LightCutOff = 70;
		m_Light.Direction = Math::TVector3F(0.1,0.0,0.0);
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("sponza.obj");
		Math::CMatrix4 transMatrix;
		transMatrix.SetScaling(0.1,0.1,0.1);
		node->LoadTransformMatrix(transMatrix);
		RootSceneGraph.AddChild(node);

		CreateSampleModel(512,512,&m_SamplePointRSM);
		CreateSampleModel(800,600,&m_SamplePointCamera);
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

	void UpdateLightWorldPos()
	{
		m_Light.Position.z = 400.0*cos(m_AngleLight*(M_PI / 180.0));
		m_Light.Position.y = 400.0*sin(m_AngleLight*(M_PI / 180.0));
		m_Light.Position.x = 0.0;

		m_Light.Direction = Math::TVector3F(0,0,0);
	}
//	void GenerateCubeBuffers(float Far = 4000.0)
//	{
//		float Near = 1.0f;
//		m_CubeFBOs = new FBOCube[5];
//
//		// Initialisation front face
//		m_CubeFBOs[0].Fbo = m_CubeShader->GetFBO();
//		m_CubeFBOs[0].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,-1),Math::TVector2F(1,1),Near, Far);
//		m_CubeFBOs[0].TransMatrix.Identity();
//
//		// Set rotation
//		m_CubeFBOs[1].TransMatrix.SetRotationX(M_PI/2.0);
//		m_CubeFBOs[2].TransMatrix.SetRotationX(-M_PI/2.0);
//		m_CubeFBOs[3].TransMatrix.SetRotationY(-M_PI/2.0);
//		m_CubeFBOs[4].TransMatrix.SetRotationY(M_PI/2.0);
//
//		// Set model for debug
//		for(int i = 0; i < 5; i++)
//			m_CubeFBOs[i].Model = 0;
//		Math::TVector2I size = SettingsManager.GetSizeRenderingWindow();
//		m_CubeFBOs[0].Model = new Rectangle2D(Math::TVector2I(0.25*size.x,0.75*size.y),Math::TVector2I(0.75*size.x,0.25*size.y));
//		m_CubeFBOs[0].Model->AddTextureMap(DIFFUSE_TEXTURE, m_CubeFBOs[0].Fbo->GetTexture("ColorBuffer"));
//		m_CubeFBOs[1].Model = new Rectangle2D(Math::TVector2I(0.25*size.x,0.25*size.y),Math::TVector2I(0.75*size.x,0.0*size.y));
//		m_CubeFBOs[2].Model = new Rectangle2D(Math::TVector2I(0.25*size.x,1.0*size.y),Math::TVector2I(0.75*size.x,0.75*size.y));
//		m_CubeFBOs[3].Model = new Rectangle2D(Math::TVector2I(0.0*size.x,0.75*size.y),Math::TVector2I(0.25*size.x,0.25*size.y));
//		m_CubeFBOs[4].Model = new Rectangle2D(Math::TVector2I(0.75*size.x,0.75*size.y),Math::TVector2I(1.0*size.x,0.25*size.y));
//
//		m_CubeFBOs[1].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,-1),Math::TVector2F(1,0),Near, Far);
//		m_CubeFBOs[2].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,0),Math::TVector2F(1,1),Near, Far);
//		m_CubeFBOs[3].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(-1,-1),Math::TVector2F(0,1),Near, Far);
//		m_CubeFBOs[4].ProjectionMatrix = Math::CMatrix4::CreatePerspectiveProjection(Math::TVector2F(0,-1),Math::TVector2F(1,1),Near, Far);
//		Math::TVector2I NewSize = m_CubeFBOs[0].Fbo->GetSize();
//		NewSize.y = NewSize.y / 2;
//		for(int i = 1; i < 5; i++)
//		{
//			m_CubeFBOs[i].Fbo = m_CubeFBOs[0].Fbo->Copy();
//			m_CubeFBOs[i].Fbo->SetSize(NewSize);
//			if(m_CubeFBOs[i].Model != 0)
//				m_CubeFBOs[i].Model->AddTextureMap(DIFFUSE_TEXTURE, m_CubeFBOs[i].Fbo->GetTexture("ColorBuffer"));
//		}
//	}

	virtual void OnUpdate(double delta)
	{}

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
					 m_ParaboloidDraw = !m_ParaboloidDraw;
					 break;
				 case SDLK_F3:
					 m_HemiDraw = !m_HemiDraw;
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		UpdateLightWorldPos();
		m_LPV.ComputeGridPosition(m_Camera);
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_GBufferShader->Begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_GBufferShader->End();

		m_Light.Compute(&RootSceneGraph);

		m_LPV.BeginInjectionGeometryPass(1);
		m_LPV.EndInjectionGeometryPass();

		m_LPV.BeginInjectionVPLPass(1);
		m_LPV.InjectVPLFromLight(m_Light, *m_SamplePointRSM);
		m_LPV.EndInjectionVPLPass();

		m_LPV.ComputePropagation(16);

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

		m_CreateMipmapping->Begin();
		m_LPVLightingAllShader->GetFBO()->GetTexture("Color")->activateMultiTex(CUSTOM_TEXTURE+0);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_LPVLightingAllShader->GetFBO()->GetTexture("Color")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_CreateMipmapping->End();

		m_GreyWorld->Begin();
		m_CreateMipmapping->GetFBO()->GetTexture("ColorBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_CreateMipmapping->GetFBO()->GetTexture("ColorBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_GreyWorld->End();

		//m_GBufferShader->GetFBO()->DrawDebug();
		//m_Light.GetFBO()->DrawDebug();
		//m_LPVLightingAllShader->GetFBO()->DrawDebug();

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

	ApplicationWhite window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
