#include "SHDOMFilePropreties.h"

#include <stdio.h>
#include <iostream>
#ifdef _DEBUG
#define DEBUG_TRACE(x) std::cout << x
#else
#define DEBUG_TRACE(x) // Nothings :)
#endif

namespace ama3D
{

SHDOMFilePropreties::~SHDOMFilePropreties()
{
	// Clean Data
	// Clean phase function data
	if (phaseCoeffs){
		for (int i=0; i<nPhaseFunctions; i++)
			if (phaseCoeffs[i])free(phaseCoeffs[i]);
		free(phaseCoeffs);
	}
	if (degreeLegendre) free(degreeLegendre);
	// Clean grid related data
	if (Zlevels)free (Zlevels);
	if (cellPhaseFuncIndex) free (cellPhaseFuncIndex);
	if (cellAlbedo) free (cellAlbedo);
	if (cellExtinctionCoeff) free (cellExtinctionCoeff);
	if (cellTemperature) free (cellTemperature);
}

void SHDOMFilePropreties::readPhaseFuncLegendreCoeffs(FILE *fl,int i)
{
	fscanf(fl,"%d",degreeLegendre+i);
	DEBUG_TRACE("[ " << degreeLegendre[i] << "]: ");
	maxDegreeLegendre = max(maxDegreeLegendre,degreeLegendre[i]);
	phaseCoeffs[i]= (float *)malloc((degreeLegendre[i]+1)*sizeof(float)); 
	phaseCoeffs[i][0] = 1.0f;
	for (int k=1; k<=degreeLegendre[i]; k++)
	{
		fscanf(fl,"%f",phaseCoeffs[i]+k);
		DEBUG_TRACE(phaseCoeffs[i][k] << " ");
	}
	DEBUG_TRACE("\n");
}
int SHDOMFilePropreties::readCellIndices(FILE *fl, int &dataIndex, bool yIgnore)
{
	int iX, iY, iZ;
	if (m_Dimension.y==1 && yIgnore){
		iY = 1;
		fscanf(fl,"%d%d",&iX,&iZ);
		DEBUG_TRACE(iX << " 1 " << iZ);
	}
	else{
		fscanf(fl,"%d%d%d",&iX,&iY,&iZ);
		DEBUG_TRACE(iX << " " << iY << " " << iZ);
	}
	iX--; iY--; iZ--;
	dataIndex = iZ + m_Dimension.z*iY + m_Dimension.z*m_Dimension.x*iX;
	return iZ;
}

SHDOMFilePropreties::SHDOMFilePropreties() :
	m_Dimension(0,0,0)
{
	nPhaseFunctions=0; 
	degreeLegendre=NULL;
	maxDegreeLegendre=0;
	phaseCoeffs=NULL; 
	delX=delY=0.0f;
	Zlevels=NULL;
	cellTemperature=NULL;
	cellExtinctionCoeff=NULL;
	cellAlbedo=NULL;
	cellPhaseFuncIndex=NULL;
}

void SHDOMFilePropreties::LoadExtinctionFile(FILE *fl)
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
	DEBUG_TRACE("Extinction Only Format.\n");
	nPhaseFunctions = 1;
	phaseCoeffs = (float **)malloc(nPhaseFunctions*sizeof(float *));
	degreeLegendre = (int *)malloc(nPhaseFunctions*sizeof(int));

	float *Zlevel_temperatures=(float *)malloc(m_Dimension.z*sizeof(float));// Temperature of the Z levels.
	for (int k=0; k<m_Dimension.z; k++){
		fscanf(fl,"%f",Zlevel_temperatures+k);
		DEBUG_TRACE(Zlevel_temperatures[k]);
	}
	DEBUG_TRACE("\n" );

	float scatteringAlbedo;
	fscanf(fl,"%f",&scatteringAlbedo);
	DEBUG_TRACE(scatteringAlbedo << " ");
	readPhaseFuncLegendreCoeffs(fl,0);

	for (int n=0; n<nCells; n++){
		int dataIndex;
		int iZ=readCellIndices(fl,dataIndex,true);
		fscanf(fl,"%f",cellExtinctionCoeff+dataIndex);
		DEBUG_TRACE( cellExtinctionCoeff[dataIndex] << "\n");
		cellTemperature[dataIndex] = Zlevel_temperatures[iZ];
		cellAlbedo[dataIndex] = scatteringAlbedo;
		cellPhaseFuncIndex[dataIndex] = 0;
	}
}

void SHDOMFilePropreties::LoadPhaseFile(FILE* fl)
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
	fscanf(fl,"%d",&nPhaseFunctions);
	DEBUG_TRACE(nPhaseFunctions << "\n");
	phaseCoeffs = (float **)malloc(nPhaseFunctions*sizeof(float *));
	degreeLegendre = (int *)malloc(nPhaseFunctions*sizeof(int));
	for (int i=0; i<nPhaseFunctions; i++)
		readPhaseFuncLegendreCoeffs(fl,i);

	for (int n=0; n<nCells; n++){
		int dataIndex;
		readCellIndices(fl,dataIndex);
		fscanf(fl,"%f",cellTemperature+dataIndex);
		fscanf(fl,"%f",cellExtinctionCoeff+dataIndex);
		fscanf(fl,"%f",cellAlbedo+dataIndex);
		fscanf(fl,"%d",cellPhaseFuncIndex+dataIndex); //phase function index (1 to NUMPHASE)
		DEBUG_TRACE(cellTemperature[dataIndex] << " " << cellExtinctionCoeff[dataIndex] << " " << cellAlbedo[dataIndex] << " " << cellPhaseFuncIndex[dataIndex] << "\n");
		cellPhaseFuncIndex[dataIndex]--;
	}
}

void SHDOMFilePropreties::LoadCommonFile(FILE* fl)
{
	int nCells = m_Dimension.x*m_Dimension.y*m_Dimension.z;
	//  Standard property file has everything variable
	//		Nx  Ny  Nz
	//		delX  delY  Z1  ...  Zn
	//		IX IY IZ Temp Extinct Albedo NumL Chi1 . . . ChiL
	//		. . .
	DEBUG_TRACE("Standard Format.\n");
	nPhaseFunctions=nCells;
	phaseCoeffs = (float **)malloc(nPhaseFunctions*sizeof(float *));
	degreeLegendre = (int *)malloc(nPhaseFunctions*sizeof(int));

	for (int n=0; n<nCells; n++){
		int dataIndex;
		readCellIndices(fl,dataIndex);
		fscanf(fl,"%f",cellTemperature+dataIndex);
		fscanf(fl,"%f",cellExtinctionCoeff+dataIndex);
		fscanf(fl,"%f",cellAlbedo+dataIndex);
		DEBUG_TRACE(cellTemperature[dataIndex] << " " << cellExtinctionCoeff[dataIndex] << " " << cellAlbedo[dataIndex] << " ");
		cellPhaseFuncIndex[dataIndex] = dataIndex;
		readPhaseFuncLegendreCoeffs(fl,dataIndex);
	}
}

SHDOMFilePropreties::FILETYPE SHDOMFilePropreties::ReadHeader(FILE* fl)
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
	fgets (firstLine, 256, fl);
	char proptype = firstLine[0];
	if (proptype == 'E')
		type = EXTINCTION_FILE;
	else if(proptype == 'T')
		type = PHASE_FILE;
	else
	{
		type = COMMON_FILE;
		rewind (fl);
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
	fscanf(fl, "%d%d%d",&m_Dimension.x, &m_Dimension.y, &m_Dimension.z);
	DEBUG_TRACE("Dimension : " << m_Dimension << "\n");
	int nCells = m_Dimension.x*m_Dimension.y*m_Dimension.z;
	cellTemperature = (float *)malloc(nCells*sizeof(float));
	cellExtinctionCoeff = (float *)malloc(nCells*sizeof(float));
	cellAlbedo = (float *)malloc(nCells*sizeof(float));
	cellPhaseFuncIndex = (int *)malloc(nCells*sizeof(int));

	fscanf(fl, "%f%f", &delX, &delY);
	DEBUG_TRACE("deltaX=" << delX <<  " deltaY=" << delY << " zLevels:");
	Zlevels=(float *)malloc(m_Dimension.z*sizeof(float));
	for (int k=0; k<m_Dimension.z; k++){
		fscanf(fl,"%f",Zlevels+k);
		DEBUG_TRACE(Zlevels[k]);
	}
	DEBUG_TRACE("\n");
	//
	// End common input
	return type;
}

bool SHDOMFilePropreties::Load(const std::string& fullpath)
{
	printf("Processing file %s\n",fullpath.c_str());

	FILE *fl = fopen(fullpath.c_str(),"rt");
	if (fl == NULL)
	{
		DEBUG_TRACE("[ERROR] Enable to found file : " << fullpath << "\n");
		return false;
	}
	
	FILETYPE type = ReadHeader(fl);

    switch(type)
    {
    	case EXTINCTION_FILE:
    		DEBUG_TRACE("[INFO] Extinction type file :)\n");
    		LoadExtinctionFile(fl);
    		break;
    	case PHASE_FILE:
    		DEBUG_TRACE("[INFO] Phase type file :)\n");
    		LoadPhaseFile(fl);
    		break;
    	case COMMON_FILE:
    		DEBUG_TRACE("[INFO] Common type file :)\n");
    		LoadCommonFile(fl);
    		break;
    	default:
    		DEBUG_TRACE("Unknow file type !");
    		fclose(fl);
    		return false;
	}

    fclose(fl);
	return true;
 }
}
