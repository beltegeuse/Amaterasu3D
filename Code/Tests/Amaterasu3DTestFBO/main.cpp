/*
 * main.cpp
 *
 *  Created on: Mar 9, 2011
 *      Author: Beltegeuse
 */

#include <iostream>
#include <Math/Matrix4.h>
#include <System/MediaManager.h>
#include <Graphics/Window.h>
#include <Graphics/GLSLShader.h>
#include <Graphics/SceneGraph/Debug/DebugCubeLeaf.h>
#include <Graphics/SceneGraph/Assimp/AssimpMesh.h>
#include <Graphics/Camera/CameraFly.h>
#include <Logger/LoggerFile.h>

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class WindowFBO : public Window
{
protected:
	Math::CMatrix4 m_matrixPerspective;
	TShaderPtr m_shader;
public:
	WindowFBO() :
		Window("Amaterasu3DTestApp")
	{
		// Camera Setup
		CameraAbstract* cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		SetCamera(cam);
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1, 10000);
		// Config path
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model/Sponza/textures");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders");
		CMediaManager::Instance().AddSearchPath("../Donnees/Shaders/TestShaders");
		// Load shader
		m_shader = CShaderManager::Instance().LoadShader("TestFBOTextureShader.shader");
		m_shader->begin();
		m_shader->setUniformMatrix4fv("ProjectionMatrix", m_matrixPerspective);
		m_shader->end();
		// Load scene
		SceneGraph::AssimpNode* node = SceneGraph::AssimpNode::LoadFromFile("dwarf.x");
		GetSceneRoot().AddChild(node);
	}

	virtual ~WindowFBO()
	{
	}

	virtual void OnDraw(double delta)
	{
		m_shader->begin();
		Window::OnDraw(delta);
		m_shader->end();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Texture* tex = m_shader->GetFBO()->GetTexture("Color");
		tex->activateTextureMapping();
		tex->activateTexture();
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
		tex->desactivateTextureMapping();
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Put into the Log system
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	std::cout << "[INFO] Begin ..." << std::endl;
	WindowFBO window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
