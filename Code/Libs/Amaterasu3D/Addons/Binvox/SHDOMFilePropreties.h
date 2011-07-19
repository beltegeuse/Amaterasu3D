#include <Math/Vector3.h>
#include <string>
#include <fstream>

namespace ama3D
{

// Cell structure
struct SHDOMCell
{
	float temperature;
	float extinction;
	float albedo;
	int phaseFuncIndex;
};

std::ostream& operator<< (std::ostream& out, const SHDOMCell c )
{
	out << "{ temp : " << c.temperature << "; ext : " << c.extinction << "; albedo : " << c.albedo << "; phaseIndex : " << c.phaseFuncIndex << " }";
	return out;
}

// Legendre coeff
struct SHDOMPhaseCoeff
{
	int NbCoeffs;
	float * Coeffs;

	SHDOMPhaseCoeff() :
		NbCoeffs(0),
		Coeffs(0)
	{
	}

	SHDOMPhaseCoeff(const SHDOMPhaseCoeff& o)
	{
		NbCoeffs = o.NbCoeffs;
		Coeffs = new float[NbCoeffs];
		for(int i = 0; i < NbCoeffs; i++)
			Coeffs[i] = o.Coeffs[i];
	}


	virtual ~SHDOMPhaseCoeff()
	{
		if(Coeffs)
		{
			delete[] Coeffs;
		}
	}

	//input:
	//	nAngles: Discrete angles in which you wish to compute the phase function
	//output:
	//	phaseFunc: Tabulated angular values. It must have preallocated space for nAngles elements.
	void GetPhaseFunction(int nAngles, float *phaseFunc)
	{
		if(!Coeffs)
			throw ama3D::CException("No data !");

		float degreeToRad = M_PI/180.0;
		for (int j = 0; j <  nAngles; j++){
			float MU = cos(degreeToRad*j*180.0/nAngles);
			float sum = 0.0;
			// Use upward recurrence to find Legendre polynomials
			float PL1 = 1.0, PL2;
			float PL = 1.0;
			for (int L = 0; L <= NbCoeffs; L++){
			  if (L > 0) PL = (2*L-1)*MU*PL1/L-(L-1)*PL2/L;
			  sum += Coeffs[L]*PL;
			  PL2 = PL1;
			  PL1 = PL ;
			}
			phaseFunc[j] = sum;
		}
	}

	float * GetPhaseFunction(int nAngles)
	{
		float * data = new float[nAngles];
		GetPhaseFunction(nAngles, data);
		return data;
	}
};

std::ostream& operator<< (std::ostream& out, const SHDOMPhaseCoeff c )
{
	out << "[ ";
	for(int i = 0; i < c.NbCoeffs; i++)
	{
		out << c.Coeffs[i];
		if(i < c.NbCoeffs - 1)
			out << "; ";
	}
	out << " ]";
	return out;
}

// File parser
class SHDOMFilePropreties{
protected:
	enum FILETYPE
	{
		COMMON_FILE,
		EXTINCTION_FILE,
		PHASE_FILE,
		UNKNOW_FILE
	};
	/*
	 * Attributs
	 */
	int m_NbPhaseFunctions; // Number of different phase  functions used in the system.
	int m_MaxDegreeLegendre;
	SHDOMPhaseCoeff* m_Coeffs;

	ama3D::Math::TVector3I m_Dimension;
	float    delX, delY, *Zlevels;
	SHDOMCell* m_Cells;
	bool m_Allocated;
	std::fstream m_file;
public:
	/*
	 * Constructors & Destructors
	 */
	SHDOMFilePropreties();
	~SHDOMFilePropreties();

	// For parsing the file
	void Load(const std::string& fullpath);
	void CleanData();

	// Global information
	bool IsAllocated() const;
	const ama3D::Math::TVector3I& GetDimension() const;

	// Get data
	const SHDOMCell& GetData(int x, int y, int z) const;
	const SHDOMCell& GetData(const ama3D::Math::TVector3I& coordinates) const;
private:
	/*
	 * Private methods
	 */
	void readPhaseFuncLegendreCoeffs(int i);
	int readCellIndices(int &dataIndex, bool yIgnore=false);

	// Get index
	int GetIndexData(int x, int y, int z) const;
	int GetIndexData(const ama3D::Math::TVector3I& coordinates) const;

	// Parser for each files
	FILETYPE ReadHeader();
	void LoadExtinctionFile();
	void LoadPhaseFile();
//	void LoadCommonFile();
};

}
