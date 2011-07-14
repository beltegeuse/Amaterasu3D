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
#include <Logger/LoggerFile.h>
#include <Graphics/Lighting/DeferredLighting/DeferredLighting.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>
#include <Addons/Logo/Logo.h>

#include <Utilities/Util.h>

using namespace ama3D;

class FastBilateralFiltering
{
private:
	/*
	 * Attributes
	 */
	// * Texture information
	Math::TVector3I m_Dimension;
	Math::TVector2I m_InputDimension;
	Math::TVector2I m_TextureRepeat;
	// * Shaders
	TShaderPtr m_InjectionStep;
	TShaderPtr m_Diffusion;
	TShaderPtr m_Slicing;
	// * Point injection
	RenderableObject* m_SamplePoint;

	// If separable kernel
	//TShaderPtr m_XDiffusion;
	//TShaderPtr m_YDiffusion;
	//TShaderPtr m_ZDiffusion;

	/*
	 * Privates methods
	 */
	void DrawQuad()
	{
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
	}

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
public:
	FastBilateralFiltering(const Math::TVector3I dimension, const Math::TVector2I& input) :
		m_Dimension(dimension),
		m_InputDimension(input)
	{
		int Taille = sqrt(dimension.z);
		m_TextureRepeat.x = NearestPowerOfTwo(Taille);
		m_TextureRepeat.y = dimension.z/m_TextureRepeat.x;
	}

	void Initialize()
	{
		CreateSampleModel(m_InputDimension.x, m_InputDimension.y, &m_SamplePoint);

		/////////////////////////
		// Shaders loading
		/////////////////////////
		m_InjectionStep = CShaderManager::Instance().LoadShader("BilateralGridInjection.shader");
		m_Slicing = CShaderManager::Instance().LoadShader("BilateralGridSlicing.shader");

		/////////////////////////
		// Texture creation
		/////////////////////////
		/*
		 * 3D texture
		 */
		Math::TVector2I sizeTex = Math::TVector2I(m_Dimension.x*m_TextureRepeat.x,m_Dimension.y*m_TextureRepeat.y);
		m_InjectionStep->GetFBO()->SetSize(sizeTex);
		m_Diffusion->GetFBO()->SetSize(sizeTex);
		/*
		 * Slicing texture
		 * rq : have the same size ...
		 */
		m_Slicing->GetFBO()->SetSize(m_InputDimension);
	}

	void Filtering(Texture* texture)
	{
		/////////////////////////
		// Injection step
		/////////////////////////
		m_InjectionStep->Begin();
		texture->activateMultiTex(CUSTOM_TEXTURE+0);
		m_SamplePoint->Draw();
		texture->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_InjectionStep->End();
		/////////////////////////
		// Diffusion
		/////////////////////////
		m_Diffusion->Begin();
		m_InjectionStep->GetFBO()->GetTexture("Result")->activateMultiTex(CUSTOM_TEXTURE+0);
		DrawQuad();
		m_InjectionStep->GetFBO()->GetTexture("Result")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_Diffusion->End();

		/////////////////////////
		// Slicing
		/////////////////////////
		m_Slicing->Begin();
		texture->activateMultiTex(CUSTOM_TEXTURE+1);
		DrawQuad();
		texture->desactivateMultiTex(CUSTOM_TEXTURE+1);
		m_Slicing->End();
	}

	Texture* GetResult()
	{
		return m_Slicing->GetFBO()->GetTexture("Result");
	}

	void DrawDebug()
	{
		m_Slicing->GetFBO()->DrawDebug();
	}
};

class ApplicationShadow : public Application
{
protected:
	FPS m_FPS;
	TTexturePtr m_BackTex;
	TShaderPtr m_Render;
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
		MatrixManager.SetProjectionMatrix(Math::CMatrix4());
		// Shader
		m_Render = ShaderManager.LoadShader("RenderTextureToRenderTarget.shader");
		// Initialise OpenGL
		GLCheck(glClearColor(0.0f,0.0f,0.0f,1.f));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Ressource loading
		m_BackTex = Texture::LoadFromFile("nature1.jpg");
	}

	virtual void OnEvent(C3::Event& event)
	{

	}

	virtual void OnUpdate(double delta)
	{}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_Render->Begin();
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
		m_Render->End();

		m_Render->GetFBO()->DrawDebug();

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
