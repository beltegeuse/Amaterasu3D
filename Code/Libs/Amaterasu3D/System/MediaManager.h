#ifndef MEDIAMANAGER_H_
#define MEDIAMANAGER_H_

#include <Singleton.h>
#include <Debug/Exceptions.h>
#include <string>
#include <vector>

struct CMediaManagerException: public CException
{
	CMediaManagerException(const std::string& File, bool directory);
};

//Todo: think about the complexity. Can store filename only and don't research at each request.
class CMediaManager
{
private:
	typedef std::vector<std::string> PathList;
	PathList m_path;
	static CMediaManager m_instance;

public:
	CMediaManager();
	virtual ~CMediaManager();

	static void AddPath(const std::string& path);
	static void AddPathAndChilds(const std::string& path);
	static const std::string GetPath(const std::string& filename);
	static void CheckPathSystem();
	static PathList GetPaths();
};

#endif /* MEDIAMANAGER_H_ */
