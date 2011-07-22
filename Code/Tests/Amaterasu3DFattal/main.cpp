#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <Math/Matrix4.h>
#include <Math/Matrix2.h>
#include <Logger/LoggerFile.h>
#include <Application.h>
#include <Graphics/Camera/CameraFPS.h>
#include <Addons/FPS/FPS.h>
#include <Addons/Binvox/VolumetricRenderableObject.h>
#include <Graphics/SceneNode/IRenderableSceneNode.h>

using namespace ama3D;

class Fattal2DVolume
{
private:
	/*
	 * Attributes
	 */
	// Shaders & FBO objects
	TShaderPtr m_FattalDisplay; //< to display I
	TShaderPtr m_FattalComputeLPM; //< to compute one wave of LPM
	TShaderPtr m_FattalUpdateBuffers; //< to update I & U
	FBO* m_FinalBuffers[2];
	// Points informations
	ISimpleRenderableSceneNode* m_InitialRays[4];
	// Topologic attributes
	Math::TVector2I m_SizeGrid;
	float m_ExtinctionCoeff;
	float m_DiffusionCoeff;
	// LPM Topologic attributes
	int m_LPMMultRes;
	int m_LPMNbAngles;
public:
	/*
	 * Constructors & Destructors
	 */
	Fattal2DVolume(const Math::TVector2I size) :
		m_SizeGrid(size),
		m_ExtinctionCoeff(0.0),
		m_DiffusionCoeff(0.01),
		m_LPMMultRes(2),
		m_LPMNbAngles(9)
	{
		// Initialise shaders
		m_FattalDisplay = CShaderManager::Instance().LoadShader("Fattal2DDisplay.shader");
		m_FattalComputeLPM = CShaderManager::Instance().LoadShader("Fattal2DLPM.shader");
		m_FattalUpdateBuffers = CShaderManager::Instance().LoadShader("Fattal2DUpdate.shader");
		// Resized buffers
		m_FattalComputeLPM->GetFBO()->SetSize(m_SizeGrid);
		m_FattalUpdateBuffers->GetFBO()->SetSize(m_SizeGrid);
		// Initialise buffers
		m_FinalBuffers[1] = m_FattalUpdateBuffers->GetFBO();
		m_FinalBuffers[2] = m_FattalUpdateBuffers->GetFBO()->Copy();
		// Initialize Rays maps
		InitializeRaysMaps();
	}

	virtual ~Fattal2DVolume()
	{}

	/*
	 * Public methods
	 */
	// Update I buffer
	void ComputeLPM(int nbPass = 3)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);
		for(int i = 0; i < nbPass; i++)
		{
			// for dir in directions:
			//   Swap buffers
			//   Compute LPM(dir)
			//   UpdateBuffers
		}
		glDisable(GL_BLEND);
	}
	// Draw I buffer
	void Render()
	{
		// Get I buffer in final
		// Bind it and draw the image
	}
private:
	void InitializeRaysMaps()
	{
		// Creation of the rays maps
		for(int i = 0; i < 4; i++) // < Foreach directions
		{
			RenderableObject::RenderableBuffer buffer;
		}
	}
};

class Fattal : public Application
{
protected:
	CameraFPS* m_Camera;
	Fattal2DVolume* m_Fattal;
public:
	Fattal()
	{
	}

	virtual ~Fattal()
	{
	}

	virtual void OnInitialize()
	{
		glPointSize(10.f);
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(30,40,20), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Create fattal
		m_Fattal = new Fattal2DVolume(Math::TVector2I(64,64));
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(MATRIX_3D);

		m_Fattal->ComputeLPM();
		m_Fattal->Render();

		MatrixManager.SetModeMatrix(MATRIX_2D);

		// Affichage du message d'aide
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
	Fattal window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
