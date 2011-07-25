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
	ISimpleRenderableSceneNode* m_InitialRaysMap[4];
	// Topologic attributes
	Math::TVector2I m_SizeGrid;
	float m_AbsortionCoeff;
	float m_DiffusionCoeff;
	// LPM Topologic attributes
	int m_LPMMultRes;
	int m_LPMNbAngles;
	// Others
	int m_IDFinalFBO;
public:
	/*
	 * Constructors & Destructors
	 */
	Fattal2DVolume(const Math::TVector2I size) :
		m_SizeGrid(size),
		m_AbsortionCoeff(0.0),
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
		m_IDFinalFBO = 0;
		// foreach pass
		for(int i = 0; i < nbPass; i++)
		{
			// foreach direction
			for(int idDir = 0; idDir < 4; idDir++)
			{
				// Set blending
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glDisable(GL_DEPTH_TEST);
				//////////////////////////
				//   Compute LPM(dir)
				//////////////////////////
				m_FattalComputeLPM->Begin();
				m_FattalComputeLPM->SetUniformVector("MainDirection", GetMainDirection(idDir));
				m_FattalComputeLPM->SetUniformVector("GridDimension",Math::TVector2F(m_SizeGrid.x,m_SizeGrid.y));
				m_FattalComputeLPM->SetUniform1f("AbsortionCoeff",m_DiffusionCoeff);
				m_FattalComputeLPM->SetUniform1f("DiffusionCoeff",m_AbsortionCoeff);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_InitialRaysMap[idDir]->Render();
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_FattalComputeLPM->End();
				glDisable(GL_BLEND);
				//////////////////////////
				//   UpdateBuffers
				//////////////////////////
				// Swap buffers
				m_FattalUpdateBuffers->SetFBO(m_FinalBuffers[(m_IDFinalFBO+1) % 2], false);
				// Add Buffers
				m_FattalUpdateBuffers->Begin();
				m_FattalUpdateBuffers->SetUniformVector("MainDirection", GetMainDirection(idDir));
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_FattalComputeLPM->GetFBO()->GetTexture("outDeltaUBuffer")->activateMultiTex(CUSTOM_TEXTURE+2);
				m_FattalComputeLPM->GetFBO()->GetTexture("outDeltaIBuffer")->activateMultiTex(CUSTOM_TEXTURE+3);

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

				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_FattalComputeLPM->GetFBO()->GetTexture("outDeltaUBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+2);
				m_FattalComputeLPM->GetFBO()->GetTexture("outDeltaIBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+3);
				m_FattalUpdateBuffers->End();
				// Update id Final
				m_IDFinalFBO = (m_IDFinalFBO+1) % 2;
			}
		}
	}
	// Draw I buffer
	void Render()
	{
		m_FattalDisplay->Begin();
		m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
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
		m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
		m_FattalDisplay->End();
	}
private:
	Math::TVector2F GetMainDirection(int idDir)
	{
		// Setup direction
		int propagationOrientation = 1;
		if(idDir < 2)
			propagationOrientation = -1;
		// Setup vector
		if((idDir % 2) == 0)
			return Math::TVector2F(propagationOrientation, 0);
		else
			return Math::TVector2F(0,propagationOrientation);
	}

	void InitializeRaysMaps()
	{
		// Generate the sampling
		Math::TVector2F* samples = new Math::TVector2F[m_LPMNbAngles];
		float offset = 1.0 / (m_LPMNbAngles+1);
		for(int i = 0; i < m_LPMNbAngles; i++)
		{
			samples[i].x = 0.5;
			samples[i].y = (i+1)*offset - 0.5;
		}

		// Creation of the rays maps
		for(int idDir = 0; idDir < 4; idDir++) // < Foreach directions
		{
			Math::TVector2F mainDir = GetMainDirection(idDir);
			// Compute Ori position
			Math::TVector2I OriPosition = Math::TVector2I(0,0);
			if(mainDir.x == -1.0 || mainDir.y == -1.0)
				OriPosition = m_SizeGrid;
			// Transformation Matrix
			Math::Matrix2 transMatrix;
			int NbCells;
			if(mainDir.x != 0)
			{
				transMatrix.a11 = mainDir.x;
				transMatrix.a22 = mainDir.x;
				NbCells = m_SizeGrid.y;
			}
			else
			{
				transMatrix.a21 = mainDir.y;
				transMatrix.a12 = mainDir.y;
				NbCells = m_SizeGrid.x;
			}
			// Main direction to generates rays
			Math::TVector2F offset(0,1);
			offset = transMatrix.Transform(offset);
			// Create all ray
			int NbRay = NbCells*m_LPMMultRes*m_LPMNbAngles;
			float* rayPosition = new float[NbRay*2];
			float* rayOrientation = new float[NbRay*2];
			float* rayValue = new float[NbRay];
			for(int k = 0; k < NbCells; k++)
			{
				for(int j = 0; j < m_LPMNbAngles; j++)
				{
					rayPosition[m_LPMNbAngles*k+j*2] = OriPosition.x+offset.x*((k+0.5)/NbCells);
					rayPosition[m_LPMNbAngles*k+j*2+1] = OriPosition.y+offset.y*((k+0.5)/NbCells);
					rayOrientation[m_LPMNbAngles*k+j*2] = transMatrix.Transform(samples[j]).x;
					rayOrientation[m_LPMNbAngles*k+j*2+1] = transMatrix.Transform(samples[j]).y;

					// TODO: Do an real initialisation
					// Initialisation Value
					if(idDir == 0)
						rayValue[m_LPMNbAngles*k+j] = 1.0;
					else
						rayValue[m_LPMNbAngles*k+j] = 0.0;
				}
			}
			// Create renderable objects
			RenderableObject::RenderableBuffer bufferPosition;
			RenderableObject::RenderableBuffer bufferDirection;
			RenderableObject::RenderableBuffer bufferValue;
			// Configure
			// * Position
			bufferPosition.buffer = rayPosition;
			bufferPosition.dimension = 2;
			bufferPosition.owner = true;
			bufferPosition.size = NbRay*2;
			// * Orientation
			bufferDirection.buffer = rayOrientation;
			bufferDirection.dimension = 2;
			bufferDirection.owner = true;
			bufferDirection.size = NbRay*2;
			// * Orientation
			bufferValue.buffer = rayValue;
			bufferValue.dimension = 1;
			bufferValue.owner = true;
			bufferValue.size = NbRay;
			// Create indice buffer
			unsigned int* indiceBuffer = new unsigned int[NbRay];
			for(int i = 0; i < NbRay; i++)
				indiceBuffer[i] = i;
			// Create object and configure
			ISimpleRenderableSceneNode * renderableObj = new ISimpleRenderableSceneNode("", 0);
			renderableObj->GetObject().SetIndiceBuffer(indiceBuffer, NbRay);
			renderableObj->GetObject().AddBuffer(bufferPosition, CUSTOM_ATTRIBUT+0);
			renderableObj->GetObject().AddBuffer(bufferDirection, CUSTOM_ATTRIBUT+1);
			renderableObj->GetObject().AddBuffer(bufferValue, CUSTOM_ATTRIBUT+2);
			m_InitialRaysMap[idDir] = renderableObj;
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
