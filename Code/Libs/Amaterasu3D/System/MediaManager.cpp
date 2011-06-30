//==========================================================
// Yes::Engine - Free C++ 3D engine
//
// Copyright (C) 2004-2005 Laurent Gomila
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : laurent.gom@gmail.com
//==========================================================

//==========================================================
// En-t�tes
//==========================================================
#include <System/MediaManager.h>
#include <boost/filesystem.hpp>
#include <System/Loaders/Loaders.h>

//////////////////////////////////////////////////////////////
//// Impl�mentation des m�thodes du singleton
//////////////////////////////////////////////////////////////
SINGLETON_IMPL(CMediaManager)

/////////////////////////////////////////////////////////////
/// Constructeur par d�faut
///
////////////////////////////////////////////////////////////
CMediaManager::CMediaManager()
{
	m_Paths.insert("");
	Logger::Log() << "[INFO] MediaManager creation .... \n";
}

/////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
CMediaManager::~CMediaManager()
{

}

/////////////////////////////////////////////////////////////
/// Ajoute un r�pertoire de recherche pour les m�dias
///
/// \param Path : Chemin � ajouter
///
////////////////////////////////////////////////////////////
void CMediaManager::AddSearchPath(const std::string& Path)
{
	if (Path.empty() || (*Path.rbegin() == '\\') || (*Path.rbegin() == '/'))
		m_Paths.insert(Path);
	else
#ifdef WIN32
		m_Paths.insert(Path + "\\");
#else
		m_Paths.insert(Path + "/");
#endif
}

void CMediaManager::AddSearchPathAndChilds(const std::string& Path)
{
	// Add the root directory
//	Logger::Log() << Path << "\n";
	AddSearchPath(Path);

	// find childs directories to add...
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(Path); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_directory(itr->status()))
		{
			AddSearchPathAndChilds(itr->path().string());
		}
	}
}

/////////////////////////////////////////////////////////////
/// Cherche un fichier dans les r�pertoires de recherche
///
/// \param Filename : Chemin du media
///
/// \return Chemin complet du media, exception si non trouv�
///
////////////////////////////////////////////////////////////
CFile CMediaManager::FindMedia(const CFile& Filename) const
{
	// Parcours de la liste des chemins de recherche
	for (std::set<std::string>::const_iterator i = m_Paths.begin();
			i != m_Paths.end(); ++i)
			{
		CFile RetFile = *i + Filename.Fullname();
		if (RetFile.Exists())
			return RetFile;
	}

	//// DEBUG
	Logger::Log() << "===== PATH : \n";
	for (std::set<std::string>::const_iterator i = m_Paths.begin();
			i != m_Paths.end(); ++i)
			{
		Logger::Log() << "  * " << *i + Filename.Fullname() << "\n";
	}

	// Si le fichier est introuvable, on lance une exception
	throw CLoadingFailed(Filename.Fullname(),
			"Fichier introuvable dans les repertoires de recherche");
}
