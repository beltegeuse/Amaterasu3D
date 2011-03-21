#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <System/SettingsManager.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/Camera/CameraFly.h>
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Graphics/MatrixManagement.h>
#include <Graphics/Font/GraphicString.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class WindowSSAO : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_gbuffer_shader;
	TShaderPtr m_SSAOBuffer;
	TTexturePtr m_NoiseTex;
	bool m_debug;
public:
	WindowSSAO() :
		Window(),
		m_debug(false)
	{
		// Camera Setup
		CameraFly* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		cam->SetSpeed(10.0);
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 0.1, 100);
		MatrixManagement::Instance().SetProjectionMatrix(m_matrixPerspective);
		// Load shader
		m_gbuffer_shader = glShaderManager::Instance().LoadShader("GBuffer.shader");
		m_SSAOBuffer = glShaderManager::Instance().LoadShader("SSAO.shader");
		// Load textures
		m_NoiseTex = Texture::LoadFromFile("noise.png");
		// Load scene
		// * Lucy loading
		SceneGraph::AssimpNode* lucyModel = SceneGraph::AssimpNode::LoadFromFile("hi_lucy.ply");
		SceneGraph::AssimpMesh* lucyMesh = (SceneGraph::AssimpMesh*)lucyModel->GetChilds()[0];
		Math::CMatrix4 lucyModelMatrix;
		lucyModelMatrix.SetScaling(1.0,1.0,1.0);
		lucyModel->LoadTransformMatrix(lucyModelMatrix);
		GetSceneRoot().AddChild(lucyModel);
		lucyMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("marble.jpg"));
		// * Scene loading
		SceneGraph::AssimpNode* sceneModel = SceneGraph::AssimpNode::LoadFromFile("uv_room.ply");
		SceneGraph::AssimpMesh* sceneMesh = (SceneGraph::AssimpMesh*)sceneModel->GetChilds()[0];
		sceneMesh->AddTextureMap(DIFFUSE_TEXTURE, Texture::LoadFromFile("bricks2_color.jpg"));
		Math::CMatrix4 sceneModelMatrix;
		sceneModelMatrix.SetScaling(5.0/3.0,5.0/3.0,5.0/3.0);
		sceneModel->LoadTransformMatrix(sceneModelMatrix);
		GetSceneRoot().AddChild(sceneModel);
	}

	virtual ~WindowSSAO()
	{
	}

	virtual void OnEvent(SDL_Event& event, double delta)
	{
		Window::OnEvent(event, delta);
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

	virtual void OnDraw(double delta)
	{
		m_gbuffer_shader->begin();
		Window::OnDraw(delta);
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
		m_gbuffer_shader->GetFBO()->GetTexture("Normal")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_gbuffer_shader->GetFBO()->GetTexture("Depth")->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_NoiseTex->desactivateMultiTex(CUSTOM_TEXTURE+2);
		m_gbuffer_shader->GetFBO()->GetTexture("Diffuse")->desactivateMultiTex(CUSTOM_TEXTURE+3);
		m_SSAOBuffer->end();

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
	SettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	// FIXME: Add auto
	CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
	std::cout << "[INFO] Begin ..." << std::endl;
	WindowSSAO window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
