//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================
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
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>
#include <Addons/Logo/Logo.h>
#include <Graphics/SceneGraph/Other/Skydome.h>

class ApplicationShadow : public Application
{
protected:
	FPS m_FPS;
	Logo m_Logo;
	TShaderPtr m_CouldSimple;
	TTexturePtr m_CouldTex;
	TTexturePtr m_BackTex;
	Math::TVector2I m_Size;

	bool m_debug;
	bool m_showDepth;
	bool m_cameraView;
public:
	ApplicationShadow() :
		m_debug(false),
		m_showDepth(false),
		m_cameraView(false)
	{
	}

	virtual void OnInitialize()
	{
		// Get infos
		m_Size = SettingsManager.GetSizeRenderingWindow();
		MatrixManager.SetProjectionMatrix(Math::CMatrix4());
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Shader loading
		m_CouldSimple = ShaderManager.LoadShader("TestCouldSimple.shader");
		// Ressource loading
		m_CouldTex = Texture::LoadFromFile("nuage.png");
		m_BackTex = Texture::LoadFromFile("random_normals.png");
	}

	virtual void OnEvent(SDL_Event& event)
	{

	}

	virtual void OnUpdate(double delta)
	{}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		glDisable(GL_DEPTH_TEST);
		m_CouldSimple->Begin();
		m_BackTex->activateMultiTex(CUSTOM_TEXTURE+0);
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
		m_BackTex->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_CouldSimple->End();

		glEnable(GL_BLEND);
		m_CouldSimple->Begin();
		m_CouldTex->activateMultiTex(CUSTOM_TEXTURE+0);
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
		m_CouldTex->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_CouldSimple->End();

		glEnable(GL_BLEND);
		m_CouldSimple->Begin();
		m_CouldTex->activateMultiTex(CUSTOM_TEXTURE+0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-0.5, -0.5);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-0.5, 0.5);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(0.5, 0.5);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(0.5, -0.5);
		glEnd();
		m_CouldTex->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_CouldSimple->End();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Draw Tow 2D things
		MatrixManager.SetModeMatrix(MATRIX_2D);

		//m_Logo.Draw();
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
	ApplicationShadow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
