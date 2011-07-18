#include <Math/Vector3.h>
#include <string>
#include <fstream>

namespace ama3D
{

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
private:
	/*
	 * Private methods
	 */
	void readPhaseFuncLegendreCoeffs(int i);
	int readCellIndices(int &dataIndex, bool yIgnore=false);

	// Parser for each files
	FILETYPE ReadHeader();
	void LoadExtinctionFile();
	void LoadPhaseFile();
//	void LoadCommonFile();
};

}
