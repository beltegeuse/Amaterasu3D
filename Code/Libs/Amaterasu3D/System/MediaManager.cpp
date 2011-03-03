#include "MediaManager.h"
#include <algorithm>
#include <boost/filesystem.hpp>


//****************************
//**** IRessource ************
//****************************
// Constructeur par défaut
IResource::IResource()
{
}
// Renvoie le nom associé à la ressource
const std::string& IResource::GetName() const
{
	return m_Name;
}
// Ajoute une référence sur la ressource
void IResource::AddRef()
{
	m_RefCount++;
}
// Retire une référence sur la ressource
int IResource::Release()
{
	m_RefCount--;
}

//****************************
//****  MediaManagerException
//****************************
MediaManagerException::MediaManagerException(const std::string& File, bool directory)
{
	if(directory)
		m_Message = "Impossible to find directory : " + File;
	else
		m_Message = "Impossible to find file : " + File;
}

//****************************
//************** MediaManager
//****************************
MediaManager::MediaManager()
{
}

MediaManager::~MediaManager()
{
}

void MediaManager::AddPath(const std::string& path)
{
	//std::cout << "[INFO] Add path : " << path << std::endl;
	// Check if the path exist
	PathList::iterator it = std::find(m_path.begin(),m_path.end(), path);
	if(it != m_path.end())
		return;

	// Check if it's a real directory
	if(!boost::filesystem::is_directory(path))
		throw MediaManagerException(path, true);

	// The path doesn't exist yet => add to list
	if(path.at(path.size()-1) == '/')
		m_path.push_back(path);
	else
		m_path.push_back(path+"/");
}

void MediaManager::AddPathAndChilds(const std::string& path)
{
	// Add the root directory
	AddPath(path);

	// find childs directories to add...
	boost::filesystem::directory_iterator end_itr;
	for ( boost::filesystem::directory_iterator itr( path ); itr != end_itr; ++itr )
	{
		if ( boost::filesystem::is_directory(itr->status()) )
		{
			AddPathAndChilds(itr->string());
		}
	}
}

const std::string MediaManager::GetPath(const std::string& filename)
{
	// search the file in all path...
	for(PathList::const_iterator it = m_path.begin(); it != m_path.end(); ++it)
	{
		std::string totalPath = (*it) + filename;
		if(boost::filesystem::exists(totalPath))
			return totalPath;
	}

	// Can find the file....
	throw MediaManagerException(filename, false);
}

void MediaManager::CheckPathSystem()
{
	//FIXME: Have to implement it
}

std::vector<std::string> MediaManager::GetPaths()
{
	return m_path;
}
