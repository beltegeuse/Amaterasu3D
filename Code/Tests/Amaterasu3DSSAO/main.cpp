#include <iostream>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>

class ApplicationSSAO : public Application
{
protected:
	CameraFPS* m_Camera;
	TShaderPtr m_gbuffer_shader;
	TShaderPtr m_SSAOBuffer;
	TShaderPtr m_BlurHShader;
	TShaderPtr m_BlurVShader;
	TTexturePtr m_NoiseTex;
	bool m_debug;
public:
	ApplicationSSAO() :
		m_debug(false)
	{

	}

	virtual ~ApplicationSSAO()
	{
	}

	virtual void OnInitialize()
	{
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(10.0);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		CMatrixManager::Instance().SetProjectionMatrix(Math::CMatrix4::CreatePerspectiveFOV(70, (double)800/600, 0.1, 100));
		// Load shader
		m_gbuffer_shader = CShaderManager::Instance().LoadShader("GBuffer.shader");
		m_SSAOBuffer = CShaderManager::Instance().LoadShader("SSAO.shader");
		m_BlurHShader = CShaderManager::Instance().LoadShader("GaussianBlurH.shader");
		m_BlurVShader = CShaderManager::Instance().LoadShader("GaussianBlurV.shader");
		// Load textures
		m_NoiseTex = Texture::LoadFromFile("random_normals.png");
		// Load scene
		// * Lucy loading
		SceneGraph::AssimpNode* lucyModel = SceneGraph::AssimpNode::LoadFromFile("hi_lucy.ply");
		SceneGraph::AssimpMesh* lucyMesh = (SceneGraph::AssimpMesh*)lucyModel->GetChilds()[0];
		Math::CMatrix4 lucyModelMatrix;
		lucyModelMatrix.SetScaling(1.0,1.0,1.0);
		lucyModel->LoadTransformMatrix(lucyModelMatrix);
		RootSceneGraph.AddChild(lucyModel);
		lucyMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("marble.jpg"));
		// * Scene loading
		SceneGraph::AssimpNode* sceneModel = SceneGraph::AssimpNode::LoadFromFile("uv_room.ply");
		SceneGraph::AssimpMesh* sceneMesh = (SceneGraph::AssimpMesh*)sceneModel->GetChilds()[0];
		sceneMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("bricks2_color.jpg"));
		Math::CMatrix4 sceneModelMatrix;
		sceneModelMatrix.SetScaling(5.0/3.0,5.0/3.0,5.0/3.0);
		sceneModel->LoadTransformMatrix(sceneModelMatrix);
		RootSceneGraph.AddChild(sceneModel);
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(SDL_Event& event)
	{
		if(event.type == SDL_KEYDOWN)
		{
			 switch(event.key.keysym.sym)
			 {
				 case SDLK_F1:
					 m_debug = !m_debug;
					 break;
			 }
		}

	}

	virtual void OnRender()
	{
		m_gbuffer_shader->begin();
		m_Camera->GetView();
		RootSceneGraph.Draw();
		m_gbuffer_shader->end();


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// SSAO Compute
		m_SSAOBuffer->begin();
		m_gbuffer_shader->GetFBO()->GetTexture("Normal")->activateMultiTex(CUSTOM_TEXTURE+0);
		m_gbuffer_shader->GetFBO()->GetTexture("Depth")->activateMultiTex(CUSTOM_TEXTURE+1);
		m_NoiseTex->activateMultiTex(CUSTOM_TEXTURE+2);
		m_gbuffer_shader->GetFBO()->GetTexture("Diffuse")->activateMultiTex(CUSTOM_TEXTURE+3);
		m_gbuffer_shader->GetFBO()->GetTexture("Position")->activateMultiTex(CUSTOM_TEXTURE+4);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_gbuffer_shader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_gbuffer_shader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_NoiseTex->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_gbuffer_shader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+3);
		m_gbuffer_shader->GetFBO()->GetTexture("Position")->desactivateMultiTex(CUSTOM_TEXTURE+4);
		m_SSAOBuffer->end();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_BlurHShader->begin();
		m_SSAOBuffer->GetFBO()->GetTexture("AmbiantOcculsion")->activateMultiTex(CUSTOM_TEXTURE+0);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_SSAOBuffer->GetFBO()->GetTexture("AmbiantOcculsion")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_BlurHShader->end();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_BlurVShader->begin();
		m_BlurHShader->GetFBO()->GetTexture("Result")->activateMultiTex(CUSTOM_TEXTURE+0);
		glBegin(GL_QUADS);
			glVertex2f(-1.0, -1.0);
			glVertex2f(-1.0, 1.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(1.0, -1.0);
		glEnd();
		m_BlurHShader->GetFBO()->GetTexture("Result")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_BlurVShader->end();

		m_BlurVShader->GetFBO()->DrawDebug();

		if(m_debug)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_gbuffer_shader->GetFBO()->DrawDebug();

			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}



		// Affichage du message d'aide
		Console.Draw();

	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
	std::cout << "[INFO] Begin ..." << std::endl;
	ApplicationSSAO window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
