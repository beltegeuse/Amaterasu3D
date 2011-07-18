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
#include <Addons/Binvox/SHDOMFilePropreties.h>

const int nFiles = 17;
std::string filenames[nFiles]={
"atmos1.prp",
"gaussol.prp",
"les2y21_sw1.prp",
"les2y21_sw2.prp",
"les2y21_sw3.prp",
"les2y21_sw4.prp",
"les2y21_sw5.prp",
"les2y21_sw6.prp",
"les2y21_sw7.prp",
"les2y21_sw8.prp",
"les2y21_sw9.prp",
"les2y21_sw10.prp",
"les2y21_sw11.prp",
"les2y21_sw12.prp",
"les2y21_sw13.prp",
"les2y21_sw14.prp",
"les2y21w16.prp"
};

class SHDOMVisualizer : public ama3D::Application
{
protected:
	/*
	 * Attributes
	 */
	ama3D::CameraFPS* m_Camera;
	ama3D::SHDOMFilePropreties m_Proprieties;

public:
	SHDOMVisualizer()
	{
	}

	virtual ~SHDOMVisualizer()
	{
	}

	virtual void OnInitialize()
	{
		glPointSize(10.f);
		// Camera Setup
		m_Camera = new ama3D::CameraFPS(ama3D::Math::TVector3F(30,40,20), ama3D::Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Load data
		ama3D::Logger::Log() << "[INFO] File need to Load ... \n";
		for (int i=0; i<nFiles; i++)
		{
			ama3D::Logger::Log() << "   * " << filenames[i] << "\n";
		}

		for (int i=0; i<nFiles; i++)
		{
			ama3D::Logger::Log() << "[Search] " << filenames[i] << " ... \n";
			ama3D::CFile proprietieFile = MediaManager.FindMedia(filenames[i]);
			m_Proprieties.Load(proprietieFile.Fullname());
			ama3D::Logger::Log() << "[FINISH] " << proprietieFile.Fullname() << " ... \n";
			m_Proprieties.CleanData();
		}
	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
	}

	virtual void OnRender()
	{
		MatrixManager.SetModeMatrix(ama3D::MATRIX_3D);

		// ... Adrien to take care of this part ...
		// Data to visualize are in global arrays:
		//	cellTemperature
		//	cellExtinctionCoeff
		//	cellAlbedo
		//	cellPhaseFuncIndex // Index to the table containing phase function
		// The size of these arrays are: nX x nY x nZ
		// Array index is computed as:
		//		iZ + nZ*iY + nZ*nY*iX;
		// where
		//	iX = 0..nX-1
		//	iY = 0..nY-1
		//	iZ = 0..nZ-1
		// Unit of Extinction coefficients are inverse of grid cell length unit.

		// Grid cells are  uniformly spaced along X and Y axes.
		// The Z axis spacing varies and exact positions are available in array: Zlevels.
		//	Index to the Zlevels array is iZ= 0..nZ-1

		// Number of phase functions used in the system is in global variable nPhaseFunctions
		// and Phase funtions are tabulated as Legendre expansion coefficients: phaseCoeffs
		// Degree of Legendre expansion varies for each phase function and available
		// in array : degreeLegendre.
		// To compute phaseFunction from coefficients use the function
		//	phasefuncFromLegendre(...)

		//// for Example.
		//
		//const int nAngles=12;
		//float phaseFunc[nAngles];
		//for (int i=0; i<p->nPhaseFunctions; i++){
		//	phasefuncFromLegendre(p->degreeLegendre[i],p->phaseCoeffs[i],nAngles,phaseFunc);
		//	printf("[%d] ",i+1);
		//	for (int j=0; j<nAngles; j++)printf("%f ",phaseFunc[j]);
		//	printf("\n");
		//}

		MatrixManager.SetModeMatrix(ama3D::MATRIX_2D);

		// Affichage du message d'aide
		Console.Draw();

	}

	void phasefuncFromLegendre( int degree, float *legCoeffs,int nAngles, float *phaseFunc)
	{
		//input:
		//	degree: degree of Legendre coefficeints
		//	legCoeffs: Coefficients array. Contains 0..degree elements.
		//	nAngles: Discreangles in which you wish to compute the phase function
		//output:
		//	phaseFunc: Tabulated anglular values. It must have preallocated spece for nAngles elements.

		float degreeToRad = M_PI/180.0;
		for (int j = 0; j <  nAngles; j++){
			float MU = cos(degreeToRad*j*180.0/nAngles);
			float sum = 0.0;
			// Use upward recurrence to find Legendre polynomials
			float PL1 = 1.0, PL2;
			float PL = 1.0;
			for (int L = 0; L <= degree; L++){
			  if (L > 0) PL = (2*L-1)*MU*PL1/L-(L-1)*PL2/L;
			  sum += legCoeffs[L]*PL;
			  PL2 = PL1;
			  PL1 = PL ;
			}
			phaseFunc[j] = sum;
		}
	}
};

#ifdef WIN32
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	ama3D::CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
	ama3D::CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
	std::cout << "[INFO] Begin ..." << std::endl;
	SHDOMVisualizer window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
