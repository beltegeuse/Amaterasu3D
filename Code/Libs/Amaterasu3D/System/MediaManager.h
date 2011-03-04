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

#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

//==========================================================
// En-têtes
//==========================================================
#include <System/Loaders/Loader.h>
#include <Singleton.h>
#include <Utilities/GenHierarchies.h>
#include <Utilities/File.h>
#include <Utilities/StringUtils.h>
#include <Utilities/SmartPtr.h>
#include <Graphics/Shader.h>
//#include <Core/Model.h>
//#include <Core/Image.h>
#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <string>

//==========================================================
// Liste des médias pris en charge
//==========================================================
typedef TYPELIST_2(Shader, ShaderUnit) MediaList;

////////////////////////////////////////////////////////////
/// Modèle de gestionnaire de média
////////////////////////////////////////////////////////////
template <class T>
struct CMediaHolder
{
	typedef std::map<std::string, CSmartPtr<ILoader<T> > > TLoadersMap;
	TLoadersMap m_Loaders; ///< Chargeurs de T associés à leur extension
};

////////////////////////////////////////////////////////////
/// Gestionnaire de médias - "empilement" de CMediaHolder pour chaque média
////////////////////////////////////////////////////////////
class CMediaManager : public CSingleton<CMediaManager>, public CScatteredHierarchy<MediaList, CMediaHolder>
{
private:
	friend class CSingleton<CMediaManager>;

public :


	//----------------------------------------------------------
	// Ajoute un répertoire de recherche pour les médias
	//----------------------------------------------------------
	void AddSearchPath(const std::string& Path);

	//----------------------------------------------------------
	// Enregistre un nouveau chargeur de T
	//----------------------------------------------------------
	template <class T> void RegisterLoader(ILoader<T>* Loader, const std::string& Extensions);

	//----------------------------------------------------------
	// Supprime un chargeur de T
	//----------------------------------------------------------
	template <class T> void UnregisterLoader(const std::string& Extensions);

	//----------------------------------------------------------
	// Charge un T à partir d'un fichier
	//----------------------------------------------------------
	template <class T> T* LoadMediaFromFile(const CFile& Filename) const;

	//----------------------------------------------------------
	// Sauvegarde un T dans un fichier
	//----------------------------------------------------------
	template <class T> void SaveMediaToFile(const T* Object, const CFile& Filename) const;

private :

	//----------------------------------------------------------
	// Constructeur par défaut
	//----------------------------------------------------------
	CMediaManager();

	//----------------------------------------------------------
	// Destructeur
	//----------------------------------------------------------
	~CMediaManager();

	//----------------------------------------------------------
	// Cherche un fichier dans les répertoires de recherche
	//----------------------------------------------------------
	CFile FindMedia(const CFile& Filename) const;

	//----------------------------------------------------------
	// Cherche le loader correspondant à une extension donnée
	//----------------------------------------------------------
	template <class T> ILoader<T>& FindLoader(const CFile& Filename) const;

	//----------------------------------------------------------
	// Données membres
	//----------------------------------------------------------
	std::set<std::string> m_Paths; ///< Liste des chemins de recherche
};

#include "MediaManager.inl"



#endif // MEDIAMANAGER_H
