#include "prop.h"

#include <stdio.h>
#include <iostream>
#ifdef _DEBUG
#define DEBUG_TRACE(x) std::cout << x
#else
#define DEBUG_TRACE(x) // Nothings :)
#endif

const int nFiles = 17;
const char *filenames[nFiles]={
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
void visualizeVolumeData(Prop *p)
{
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
}

Prop::~Prop()
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

void Prop::readPhaseFuncLegendreCoeffs(FILE *fl,int i)
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
int Prop::readCellIndices(FILE *fl, int &dataIndex, bool yIgnore)
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

Prop::Prop() :
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
bool Prop::parsePropFile(const char *propfilename)
{
	printf("Processing file %s\n",propfilename);
    // SUBROUTINE READ_PROPERTY_SIZE (PROPFILE, NLEG, NPX, NPY, NPZ, 
    //.                            NUMPHASE, MAXLEG, MAXPGL, DELX, DELY)
	// Get grid property size	      
	//       Reads parts of the property file to get the maximum array sizes
	//     needed for allocatable arrays.  For extinction only and tabulated
	//     phase function formats, only the header is read, while for the
	//     standard format, the whole file must be read to determine MAXLEG.

	//          Open the file, figure out the type, and get the grid size
    FILE *fl = fopen(propfilename,"rt");
	if (fl == NULL) return false;

    char firstLine[256];
	fgets (firstLine, 256, fl);
	char proptype = firstLine[0];
    if (proptype != 'E' && proptype != 'T')  rewind (fl);

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

	
    if (proptype == 'E')
	{
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
	else if (proptype == 'T')
	{
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
	else
	{
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
	fclose(fl);
	return true;
 }

//int main(int argc,char *argv[])
//{
//	Prop *prop;
//	for (int i=0; i<nFiles; i++)
//	{
//		prop=new Prop();
//		if (prop->parsePropFile(filenames[i])){
//			visualizeVolumeData(prop);
//		}
//		delete prop;
//	}
//	return (0);
//}
