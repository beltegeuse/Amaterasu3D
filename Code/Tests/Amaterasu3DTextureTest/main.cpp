/*
 * main.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: Adrien
 */

#include <Graphics/Window.h>
#include <Graphics/Texture.h>
#include <Graphics/Camera/CameraFly.h>
#include <Debug/OpenGLDebug.h>
#include <System/MediaManager.h>
#include <iostream>
#include <windows.h>

class ConcreteWindow : public Window
{
private:
	Math::CMatrix4 m_matrixPerspective;
	TTexturePtr m_texture;
	CameraAbstract* m_cam;
public:
	ConcreteWindow() :
		Window("Amaterasu3DTestApp")
	{
		// Camera
		m_cam = new CameraFly(Math::TVector3F(3,4,2), Math::TVector3F(0,0,0));
		SetCamera(m_cam);
		// OpenGL Flags ...
		GLCheck(glClearColor(0.1f,0.1f,0.1f,1.f));
		m_matrixPerspective.PerspectiveFOV(70, (double)800/600, 1, 10000);
		// Path search
		// Load the Shader
		CMediaManager::Instance().AddSearchPath("../Donnees");
		CMediaManager::Instance().AddSearchPath("../Donnees/Model");
		// Load texture
		m_texture = Texture::LoadFromFile("unknowTexture.tga");
		std::cout << "FINISH BUILD SCENE" << std::endl;
	}

	virtual ~ConcreteWindow()
	{
	}

	virtual void OnDraw()
	{
		// Compute ModelViewMatrix
		// Send matrix to the shader
		// Draw the geometry
		//Window::OnDraw();
		// End of the shader
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_texture->activateTextureMapping();
		m_texture->activateTexture();
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

		m_texture->desactivateTextureMapping();

		//recursive_render(m_scene, m_scene->mRootNode);
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::cout << "[INFO] Begin ..." << std::endl;
	ConcreteWindow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
