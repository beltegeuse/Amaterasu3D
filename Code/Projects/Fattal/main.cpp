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

#define DEBUGFATTAL 0

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
	Math::TVector2F m_CellSize;
	float m_AbsortionCoeff;
	float m_ScatteringCoeff;
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
		m_ScatteringCoeff(0.01),
		m_LPMMultRes(2),
		m_LPMNbAngles(9)
	{
		// Initialise shaders
		m_FattalDisplay = CShaderManager::Instance().LoadShader("Fattal2DDisplay.shader");
#if DEBUGFATTAL
		m_FattalComputeLPM = CShaderManager::Instance().LoadShader("Fattal2DLPM_Debug.shader");
#else
		m_FattalComputeLPM = CShaderManager::Instance().LoadShader("Fattal2DLPM.shader");
#endif
		m_FattalUpdateBuffers = CShaderManager::Instance().LoadShader("Fattal2DUpdate.shader");
		// Compute
		Math::TVector2I screenSize = CSettingsManager::Instance().GetSizeRenderingWindow();
		m_CellSize = Math::TVector2F(screenSize.x/(float)size.x, screenSize.y/(float)size.y);
		// Resized buffers
		// FIXME
#if !DEBUGFATTAL
		m_FattalComputeLPM->GetFBO()->SetSize(m_SizeGrid);
		m_FattalUpdateBuffers->GetFBO()->SetSize(m_SizeGrid);
#endif
		// Initialise buffers
		m_FinalBuffers[0] = m_FattalUpdateBuffers->GetFBO();
		m_FinalBuffers[1] = m_FattalUpdateBuffers->GetFBO()->Copy();

		// Initialize Rays maps
		InitializeRaysMaps();

		Logger::Log().Sync();
	}

	virtual ~Fattal2DVolume()
	{}

	void ClearBuffers()
	{
		m_FinalBuffers[0]->Bind();
		m_FinalBuffers[0]->UnBind();
		m_FinalBuffers[1]->Bind();
		m_FinalBuffers[1]->UnBind();
	}

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
//#if DEBUGFATTAL
//			int idDir = 2;
//#else
			for(int idDir = 0; idDir < 4; idDir++)
//#endif
			{
				// Set blending
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				//////////////////////////
				//   Compute LPM(dir)
				//////////////////////////
				m_FattalComputeLPM->Begin();
				// ===== Uniform setters
//				std::cout << GetMainDirection(idDir) << std::endl;
//				std::cout << Math::TVector2F(m_SizeGrid.x,m_SizeGrid.y) << std::endl;
				m_FattalComputeLPM->SetUniformVector("MainDirection", GetMainDirection(idDir));
				m_FattalComputeLPM->SetUniformVector("GridDimension",Math::TVector2F(m_SizeGrid.x,m_SizeGrid.y));
				m_FattalComputeLPM->SetUniformVector("CellDimension", m_CellSize);
				m_FattalComputeLPM->SetUniform1f("AbsortionCoeff",m_AbsortionCoeff);
				m_FattalComputeLPM->SetUniform1f("ScaterringCoef",m_ScatteringCoeff);
				m_FattalComputeLPM->SetUniform1i("isFristSweep", i == 0);
				// ==== Texture activation
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
				// ==== Drawing (Attributes)
				m_InitialRaysMap[idDir]->Render();
				// ==== Texture desactivation
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_FattalComputeLPM->End();
				glDisable(GL_BLEND);
				//////////////////////////
				//   UpdateBuffers
				//////////////////////////
#if !DEBUGFATTAL
				// Use swap buffer technique
				m_FattalUpdateBuffers->SetFBO(m_FinalBuffers[(m_IDFinalFBO+1) % 2], false);
				// Add Buffers
				m_FattalUpdateBuffers->Begin();
				// ==== Uniform
				m_FattalUpdateBuffers->SetUniformVector("MainDirection", GetMainDirection(idDir));
				// ==== Texture activation
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->activateMultiTex(CUSTOM_TEXTURE+0);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->activateMultiTex(CUSTOM_TEXTURE+1);
				m_FattalComputeLPM->GetFBO()->GetTexture("outDeltaBuffer")->activateMultiTex(CUSTOM_TEXTURE+2);
				// ==== Drawing
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
				// ==== Texture desactivation
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outUBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+0);
				m_FinalBuffers[m_IDFinalFBO]->GetTexture("outIBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+1);
				m_FattalComputeLPM->GetFBO()->GetTexture("outDeltaBuffer")->desactivateMultiTex(CUSTOM_TEXTURE+2);
				m_FattalUpdateBuffers->End();
#endif
				// Update id Final
				m_IDFinalFBO = (m_IDFinalFBO+1) % 2;
			}
		}
	}
	// Draw I buffer
	void Render()
	{

		// Show result
#if DEBUGFATTAL
		m_FattalComputeLPM->GetFBO()->DrawDebug();
#else
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
		//m_FinalBuffers[m_IDFinalFBO]->DrawDebug();
#endif
	}
private:
	Math::TVector2F GetMainDirection(int idDir)
	{
		// Setup direction
		int propagationOrientation = 1;
		if((idDir % 2) == 0)
			propagationOrientation = -1;
		// Setup vector
		if(idDir < 2)
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
			samples[i].Normalize();
		}

		// Creation of the rays maps
		for(int idDir = 0; idDir < 4; idDir++) // < Foreach directions
		{
			Math::TVector2F mainDir = GetMainDirection(idDir);
			// Compute Ori position
			Math::TVector2I OriPosition = Math::TVector2I(0,0);
			if(mainDir.x == -1.0 || mainDir.y == -1.0)
				OriPosition = Math::TVector2I(m_SizeGrid.x*m_CellSize.x,m_SizeGrid.y*m_CellSize.y);
			// Transformation Matrix
			Math::Matrix2 transMatrix(0,0,0,0);
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
			int NbGroupRays = NbCells*m_LPMMultRes;
			int NbRay = NbGroupRays*m_LPMNbAngles;
			float* rayPosition = new float[NbRay*2];
			float* rayOrientation = new float[NbRay*2];
			float* rayValue = new float[NbRay];
			// For ray initialisation
			int beamLow = NbGroupRays/2 - NbGroupRays*0.025;
			int beamHigh = NbGroupRays/2 + NbGroupRays*0.025 + 1;
			for(int k = 0; k < NbGroupRays; k++)
			{
				for(int j = 0; j < m_LPMNbAngles; j++)
				{
					int indice = (m_LPMNbAngles*k+j)*2;
					float factor = (k+0.5)/m_LPMMultRes;
					// FIXME
					rayPosition[indice] = OriPosition.x+offset.x*factor*m_CellSize.x;
					rayPosition[indice+1] = OriPosition.y+offset.y*factor*m_CellSize.y;
					rayOrientation[indice] = transMatrix.Transform(samples[j]).x;
					rayOrientation[indice+1] = transMatrix.Transform(samples[j]).y;

					// TODO: Do an real initialisation
					// Debug
#if DEBUGFATTAL
					rayValue[m_LPMNbAngles*k+j] = 1.0;
#else
					// Initialisation Value
					if(j == (m_LPMNbAngles/2) && k >= beamLow && k <= beamHigh && idDir == 0) // &&
						rayValue[m_LPMNbAngles*k+j] = 10.0;
					else
						rayValue[m_LPMNbAngles*k+j] = 0.0;
#endif
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
			renderableObj->GetObject().SetDrawMode(GL_POINTS);
			renderableObj->GetObject().SetIndiceBuffer(indiceBuffer, NbRay);
			renderableObj->GetObject().AddBuffer(bufferPosition, CUSTOM_ATTRIBUT+0);
			renderableObj->GetObject().AddBuffer(bufferDirection, CUSTOM_ATTRIBUT+1);
			renderableObj->GetObject().AddBuffer(bufferValue, CUSTOM_ATTRIBUT+2);
			renderableObj->GetObject().CompileBuffers();
			m_InitialRaysMap[idDir] = renderableObj;
		}
	}
};

class Fattal : public Application
{
protected:
	CameraFPS* m_Camera;
	Fattal2DVolume* m_Fattal;
	FPS m_FPSCounter;
public:
	Fattal()
	{
	}

	virtual ~Fattal()
	{
	}

	virtual void OnInitialize()
	{
		//glPointSize(1.f);
		// Camera Setup
		m_Camera = new CameraFPS(Math::TVector3F(30,40,20), Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Create fattal
#if DEBUGFATTAL
		m_Fattal = new Fattal2DVolume(Math::TVector2I(10,10));
#else
		m_Fattal = new Fattal2DVolume(Math::TVector2I(64,64));
#endif
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

		m_Fattal->ClearBuffers();
		m_Fattal->ComputeLPM();
		m_Fattal->Render();

		MatrixManager.SetModeMatrix(MATRIX_2D);

		// Affichage du message d'aide
		Console.Draw();

	}
};

int main()
{
	CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	CFontManager::Instance().LoadFont("Cheeseburger_ttf.font");
	std::cout << "[INFO] Begin ..." << std::endl;
	Fattal window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
