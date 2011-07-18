#include <Math/Vector3.h>
#include <string>
#include <fstream>

namespace ama3D
{

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
	int *m_DegreeLegendre;
	int m_MaxDegreeLegendre;
	float **m_PhaseCoeffs; 

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

	// Get data
	bool IsAllocated() const;
	const ama3D::Math::TVector3I& GetDimension() const;
	int GetIndexData(int x, int y, int z) const;
	int GetIndexData(const ama3D::Math::TVector3I& coordinates) const;
	const SHDOMCell& GetData(int x, int y, int z) const;
	const SHDOMCell& GetData(const ama3D::Math::TVector3I& coordinates) const;
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
