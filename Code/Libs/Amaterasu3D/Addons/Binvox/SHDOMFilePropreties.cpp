#include "SHDOMFilePropreties.h"
#include <algorithm>

#ifdef _DEBUG
#define DEBUG_TRACE(x) std::cout << x
#else
#define DEBUG_TRACE(x) // Nothings :)
#endif

namespace ama3D
{

SHDOMFilePropreties::~SHDOMFilePropreties()
{
	CleanData();
}

void SHDOMFilePropreties::readPhaseFuncLegendreCoeffs(int i)
{
	m_file >> m_DegreeLegendre[i];
	DEBUG_TRACE("[ " << m_DegreeLegendre[i] << "]: ");
	m_MaxDegreeLegendre = std::max(m_MaxDegreeLegendre,m_DegreeLegendre[i]);
	m_PhaseCoeffs[i]= new float[(m_DegreeLegendre[i]+1)];
	m_PhaseCoeffs[i][0] = 1.0f;
	for (int k=1; k<=m_DegreeLegendre[i]; k++)
	{
		m_file >> m_PhaseCoeffs[i][k];
		DEBUG_TRACE(m_PhaseCoeffs[i][k] << " ");
	}
	DEBUG_TRACE("\n");
}

int SHDOMFilePropreties::readCellIndices(int &dataIndex, bool yIgnore)
{
	int iX, iY, iZ;
	if (m_Dimension.y==1 && yIgnore){
		iY = 1;
		m_file >> iX >> iZ;
		DEBUG_TRACE("Indice " << iX << " 1 " << iZ);
	}
	else{
		m_file >> iX >> iY >> iZ;
		DEBUG_TRACE("Indice " << iX << " " << iY << " " << iZ);
	}
	iX--; iY--; iZ--;
	dataIndex = iX + m_Dimension.x*iY + m_Dimension.x*m_Dimension.y*iZ;
	return iZ;
}

SHDOMFilePropreties::SHDOMFilePropreties() :
	m_Dimension(0,0,0),
	m_Cells(0),
	m_Allocated(false)
{
	m_NbPhaseFunctions=0; 
	m_DegreeLegendre=NULL;
	m_MaxDegreeLegendre=0;
	m_PhaseCoeffs=NULL; 
	delX=delY=0.0f;
	Zlevels=NULL;
}

void SHDOMFilePropreties::LoadExtinctionFile()
{
	int nCells = m_Dimension.x*m_Dimension.y*m_Dimension.z;
	//	Property file type E is for extinction only format
	//	Simplest of the input.
	//		only one phase function.
	//	E
	//	Nx  Ny  Nz
	//	delX  delY  Z1 ... Zn
	//	T1 ... Tn
	//	Albedo  NumL  Chi1 ...  ChiL
	//	IX IZ Extinct   or   IX IY IZ Extinct
	//  . . .
	m_NbPhaseFunctions = 1;
	m_PhaseCoeffs = new float*[m_NbPhaseFunctions];
	m_DegreeLegendre = new int[m_NbPhaseFunctions];

	float *Zlevel_temperatures=new float[m_Dimension.z];// Temperature of the Z levels.
	for (int k=0; k<m_Dimension.z; k++){
		m_file >> Zlevel_temperatures[k];
		DEBUG_TRACE(Zlevel_temperatures[k] << " ");
	}
	DEBUG_TRACE("\n" );

	float scatteringAlbedo;
	m_file >> scatteringAlbedo;
	DEBUG_TRACE(scatteringAlbedo << " ");
	readPhaseFuncLegendreCoeffs(0);

	DEBUG_TRACE("Read Data (nb cell : " << nCells << ") \n");
	for (int n=0; n<nCells; n++){
		int dataIndex;
		int iZ=readCellIndices(dataIndex,true);
		DEBUG_TRACE( " | Data index : " << dataIndex);
		m_file >> m_Cells[dataIndex].extinction;
		m_Cells[dataIndex].temperature = Zlevel_temperatures[iZ];
		m_Cells[dataIndex].albedo = scatteringAlbedo;
		m_Cells[dataIndex].phaseFuncIndex = 0;
		DEBUG_TRACE( " | " << m_Cells[dataIndex] << "\n");
	}
}

void SHDOMFilePropreties::LoadPhaseFile()
{
	int nCells = m_Dimension.x*m_Dimension.y*m_Dimension.z;
	// Property file type T is for tabulated phase function format
	//	T
	//	Nx  Ny  Nz
	//	delX  delY  Z1 ... Zn
	//	numphase
	//	NumL  Chi1 ...  ChiL       (for each phase function)
	//  . . .
	//	IX IY IZ  Temp Extinct Albedo  Iphase
	//  . . .
	DEBUG_TRACE("Tabulated Phase Function Format.\n");
	m_file >> m_NbPhaseFunctions;
	DEBUG_TRACE(m_NbPhaseFunctions << "\n");
	m_PhaseCoeffs = new float*[m_NbPhaseFunctions];
	m_DegreeLegendre = new int[m_NbPhaseFunctions];

	for (int i=0; i<m_NbPhaseFunctions; i++)
		readPhaseFuncLegendreCoeffs(i);

	for (int n=0; n<nCells; n++){
		int dataIndex;
		readCellIndices(dataIndex);
		m_file >> m_Cells[dataIndex].temperature;
		m_file >> m_Cells[dataIndex].extinction;
		m_file >> m_Cells[dataIndex].albedo;
		m_file >> m_Cells[dataIndex].phaseFuncIndex; //phase function index (1 to NUMPHASE)
		m_Cells[dataIndex].phaseFuncIndex--;
		DEBUG_TRACE(" | " << m_Cells[dataIndex] << "\n");
	}
}

//void SHDOMFilePropreties::LoadCommonFile()
//{
//	int nCells = m_Dimension.x*m_Dimension.y*m_Dimension.z;
//	//  Standard property file has everything variable
//	//		Nx  Ny  Nz
//	//		delX  delY  Z1  ...  Zn
//	//		IX IY IZ Temp Extinct Albedo NumL Chi1 . . . ChiL
//	//		. . .
//	DEBUG_TRACE("Standard Format.\n");
//	nPhaseFunctions=nCells;
//	phaseCoeffs = new float*[nPhaseFunctions];
//	degreeLegendre = new int[nPhaseFunctions];
//
//	for (int n=0; n<nCells; n++){
//		int dataIndex;
//		readCellIndices(fl,dataIndex);
//		fscanf(fl,"%f",cellTemperature+dataIndex);
//		fscanf(fl,"%f",cellExtinctionCoeff+dataIndex);
//		fscanf(fl,"%f",cellAlbedo+dataIndex);
//		DEBUG_TRACE(cellTemperature[dataIndex] << " " << cellExtinctionCoeff[dataIndex] << " " << cellAlbedo[dataIndex] << " ");
//		cellPhaseFuncIndex[dataIndex] = dataIndex;
//		readPhaseFuncLegendreCoeffs(fl,dataIndex);
//	}
//}

SHDOMFilePropreties::FILETYPE SHDOMFilePropreties::ReadHeader()
{
	FILETYPE type = UNKNOW_FILE;
	// SUBROUTINE READ_PROPERTY_SIZE (PROPFILE, NLEG, NPX, NPY, NPZ,
	//.                            NUMPHASE, MAXLEG, MAXPGL, DELX, DELY)
	// Get grid property size
	//       Reads parts of the property file to get the maximum array sizes
	//     needed for allocatable arrays.  For extinction only and tabulated
	//     phase function formats, only the header is read, while for the
	//     standard format, the whole file must be read to determine MAXLEG.

	//          Open the file, figure out the type, and get the grid size
	char firstLine[256];
	m_file.getline(firstLine,256);
	char proptype = firstLine[0];
	if (proptype == 'E')
		type = EXTINCTION_FILE;
	else if(proptype == 'T')
		type = PHASE_FILE;
	else
	{
		type = COMMON_FILE;
		m_file.seekg (0, std::ios::beg);
	}
	// Begin List of Data read from the file.
	//
	//phaseCoeffs: The Legendre coefficients of the phase function.
		//		They are normalized such that 0th coefficient is always 1,
		//		and 1th coefficient is 3 times the asymmetry parameter (g).
	//
	// End List of data.


	// Following scans are common for all input types.
	// Data read are:
	//	Nx  Ny  Nz
	//	delX  delY  Z1 ... Zn
	//
	// Begin common input
	//
	m_file >> m_Dimension.x >> m_Dimension.y >> m_Dimension.z;
	DEBUG_TRACE("Dimension : " << m_Dimension << "\n");
	int nCells = m_Dimension.x*m_Dimension.y*m_Dimension.z;
	m_Cells = new SHDOMCell[nCells];

	m_file >> delX >> delY;
	DEBUG_TRACE("deltaX=" << delX <<  " deltaY=" << delY << " zLevels:");
	Zlevels=new float[m_Dimension.z];
	for (int k=0; k<m_Dimension.z; k++){
		m_file >> Zlevels[k];
		DEBUG_TRACE(Zlevels[k]);
	}
	DEBUG_TRACE("\n");
	//
	// End common input
	return type;
}

void SHDOMFilePropreties::Load(const std::string& fullpath)
{
	printf("Processing file %s\n",fullpath.c_str());

	if(m_Allocated)
	{
		throw CException("Already allocated !");
	}

	m_file.open(fullpath.c_str(), std::fstream::in);
	if (!m_file.is_open())
	{
		throw CException("Unenable to found file : " + fullpath);
	}
	
	FILETYPE type = ReadHeader();

    switch(type)
    {
    	case EXTINCTION_FILE:
    		DEBUG_TRACE("[INFO] Extinction type file :)\n");
    		LoadExtinctionFile();
    		break;
    	case PHASE_FILE:
    		DEBUG_TRACE("[INFO] Phase type file :)\n");
    		LoadPhaseFile();
    		break;
//    	case COMMON_FILE:
//    		DEBUG_TRACE("[INFO] Common type file :)\n");
//    		LoadCommonFile();
//    		break;
    	default:
    		m_file.close();
    		throw CException("Unknow file type !");
	}

    m_Allocated = true;

    //DEBUG_TRACE("FIN " << fullpath << "\n");
    m_file.close();
}

void SHDOMFilePropreties::CleanData()
{
	// Clean Data
	// Clean phase function data
	if (m_PhaseCoeffs)
	{
		for (int i=0; i<m_NbPhaseFunctions; i++)
		{
			if (m_PhaseCoeffs[i])
				delete [] m_PhaseCoeffs[i];
		}
		delete [] m_PhaseCoeffs;
		m_PhaseCoeffs = 0;
	}
	if (m_DegreeLegendre)
	{
		delete[] m_DegreeLegendre;
		m_DegreeLegendre = 0;
	}
	// Clean grid related data
	if (Zlevels)
	{
		delete[] Zlevels;
		Zlevels = 0;
	}

	if (m_Cells)
	{
		delete[] m_Cells;
		m_Cells = 0;
	}

	 m_Allocated = false;
}

} // Namespace ama3D
