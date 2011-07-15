#include <Math/Vector3.h>

#include <malloc.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define max(a,b) ((a>b)?a:b)

namespace ama3D
{

class SHDOMFilePropreties{
protected:
	/*
	 * Attributs
	 */
	int nPhaseFunctions; // Number of different phase  functions used in the system.
	int *degreeLegendre;
	int maxDegreeLegendre;
	float **phaseCoeffs; 

	ama3D::Math::TVector3I m_Dimension;
	float    delX, delY, *Zlevels;
	float *cellTemperature;
	float *cellExtinctionCoeff;
	float *cellAlbedo;
	int   *cellPhaseFuncIndex;

public:
	/*
	 * Constructors & Destructors
	 */
	SHDOMFilePropreties();
	~SHDOMFilePropreties();

	// For parsing the file
	bool parsePropFile(const char *propfilename);
private:
	/*
	 * Private methods
	 */
	void readPhaseFuncLegendreCoeffs(FILE *fl,int i);
	int readCellIndices(FILE *fl, int &dataIndex, bool yIgnore=false);
};

}
