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

#include <boost/program_options.hpp>
namespace po = boost::program_options;


class SHDOMVisualizer : public ama3D::Application
{
protected:
	/*
	 * Attributes
	 */
	ama3D::CameraFPS* m_Camera;
	ama3D::SHDOMFilePropreties m_Proprieties;
	ama3D::SHDOMRenderableObj* m_Volume;
	ama3D::SHDOMRenderableObj::RENDERABLE_TYPE m_type;
	bool m_NeedUpdate;
public:
	SHDOMVisualizer() :
		m_NeedUpdate(false),
		m_Volume(0)
	{
	}

	virtual ~SHDOMVisualizer()
	{
	}

	void Load(const std::string& file, ama3D::SHDOMRenderableObj::RENDERABLE_TYPE type = ama3D::SHDOMRenderableObj::EXTINCTION)
	{
		if(m_Proprieties.IsAllocated())
			m_Proprieties.CleanData();

		ama3D::Logger::Log() << "[INFO] File need to Load ... \n";
		ama3D::Logger::Log() << "[Search] " << file << " ... \n";
		ama3D::CFile proprietieFile = MediaManager.FindMedia(file);
		m_Proprieties.Load(proprietieFile.Fullname());
		ama3D::Logger::Log() << "[FINISH] " << proprietieFile.Fullname() << " ... \n";
		m_NeedUpdate = true;
		m_type = type;
	}

	virtual void OnInitialize()
	{
		glPointSize(10.f);
		// Camera Setup
		m_Camera = new ama3D::CameraFPS(ama3D::Math::TVector3F(30,40,20), ama3D::Math::TVector3F(0,0,0));
		m_Camera->SetSpeed(100.0);
		// Shader loading ...

	}

	virtual void OnUpdate(double delta)
	{
	}

	virtual void OnEvent(C3::Event& event)
	{
	}

	virtual void OnRender()
	{
		if(m_NeedUpdate)
		{
			if(m_Volume)
				delete m_Volume;
			m_Volume = new ama3D::SHDOMRenderableObj(m_type, &m_Proprieties);
			m_NeedUpdate = false;
		}

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

		//iX + m_Dimension.x*iY + m_Dimension.x*m_Dimension.y*iZ

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

		m_Volume->UpdateCubeFBO(m_Camera);
		m_Volume->Render();

		MatrixManager.SetModeMatrix(ama3D::MATRIX_2D);

		// Affichage du message d'aide
		Console.Draw();

	}
};

//#ifdef WIN32
//#include <windows.h>
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//#else
int main(int argc, char *argv[])
//#endif
{
	po::options_description desc("Program Usage", 1024, 512);
	try
	{
		std::string file;

		desc.add_options()
		  ("help,h",     "produce help message")
		  ("file,f",  po::value<std::string>(&file)->required(),   "set file to parse")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return -1;
		}

		po::notify(vm);

		ama3D::CSettingsManager::Instance().LoadFile("../Donnees/Config.xml");
		ama3D::CFontManager::Instance().LoadFont("../Donnees/Fonts/Cheeseburger.ttf", "arial");
		std::cout << "[INFO] Begin ..." << std::endl;
		SHDOMVisualizer window;
		window.Load(file);
		window.Run();
		std::cout << "[INFO] ... end." << std::endl;

	}
	catch(std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		std::cout << desc << "\n";
		return -1;
	}

	return 0;
}
