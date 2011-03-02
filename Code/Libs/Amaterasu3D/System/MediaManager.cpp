#include "MediaManager.h"
#include <algorithm>
#include <boost/filesystem.hpp>

//******************************** CMediaManagerException
CMediaManagerException::CMediaManagerException(const std::string& File, bool directory)
{
	if(directory)
		m_Message = "Impossible to find directory : " + File;
	else
		m_Message = "Impossible to find file : " + File;
}

//******************************** CMediaManager
CMediaManager CMediaManager::m_instance;

CMediaManager::CMediaManager()
{
}

CMediaManager::~CMediaManager()
{
}

void CMediaManager::AddPath(const std::string& path)
{
	// Check if the path exist
	PathList::iterator it = std::find(m_instance.m_path.begin(),m_instance.m_path.end(), path);
	if(it != m_instance.m_path.end())
		return;

	// Check if it's a real directory
	if(!boost::filesystem::is_directory(path))
		throw CMediaManagerException(path, true);

	// The path doesn't exist yet => add to list
	m_instance.m_path.push_back(path);
}

void CMediaManager::AddPathAndChilds(const std::string& path)
{
	// Add the root directory
	AddPath(path);

	// find childs directories to add...
	boost::filesystem::directory_iterator end_itr;
	for ( boost::filesystem::directory_iterator itr( path ); itr != end_itr; ++itr )
	{
		if ( boost::filesystem::is_directory(itr->status()) )
		{
			AddPathAndChilds(path);
		}
	}
}

const std::string CMediaManager::GetPath(const std::string& filename)
{
	// seqrch the file in all path...
	for(PathList::const_iterator it = m_instance.m_path.begin(); it != m_instance.m_path.end(); ++it)
	{
		std::string totalPath = (*it) + filename;
		if(boost::filesystem::exists(totalPath))
			return totalPath;
	}

	// Can find the file....
	throw CMediaManagerException(filename, false);
}

void CMediaManager::CheckPathSystem()
{
	//FIXME: Have to implement it
}

std::vector<std::string> CMediaManager::GetPaths()
{
	return m_instance.m_path;
}
