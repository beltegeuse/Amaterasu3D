#include "MediaManager.h"

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
}

void CMediaManager::AddPathAndChilds(const std::string& path)
{

}

const std::string CMediaManager::GetPath(const std::string& filename)
{
	return "";
}

void CMediaManager::CheckPathSystem()
{

}

std::vector<std::string> CMediaManager::GetPaths()
{
	return m_instance.m_path;
}
