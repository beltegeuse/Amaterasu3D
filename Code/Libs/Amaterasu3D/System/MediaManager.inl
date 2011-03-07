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


/////////////////////////////////////////////////////////////
/// Enregistre un nouveau chargeur de media de type T
///
/// \param Loader :     Pointeur sur le chargeur
/// \param Extensions : Extensions associées au chargeur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CMediaManager::RegisterLoader(ILoader<T>* Loader, const std::string& Extensions)
{
    // Récupération des extensions
    std::vector<std::string> Ext;
    Split(Extensions, Ext, " /\\*.,;|-_\t\n'\"");

	Logger::Log() << "[INFO] Add New Loader ... \n";

    // Ajout des extensions une à une
    CSmartPtr<ILoader<T> > Ptr = Loader;
    for (std::vector<std::string>::iterator i = Ext.begin(); i != Ext.end(); ++i)
	{
		Logger::Log() << "  * " << ToLower(*i) << " ( " << CMediaHolder<T>::m_Loaders.size() << ")\n";
        CMediaHolder<T>::m_Loaders[ToLower(*i)] = Ptr;
	}
}


////////////////////////////////////////////////////////////
/// Supprime un chargeur
///
/// \param Extensions : Extensions associées au chargeur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CMediaManager::UnregisterLoader(const std::string& Extensions)
{
    // Récupération des extensions
    std::vector<std::string> Ext;
    Split(Extensions, Ext, " /\\*.,;|-_\t\n'\"");

    // Suppression du chargeur pour chaque extension
    for (std::vector<std::string>::iterator i = Ext.begin(); i != Ext.end(); ++i)
        CMediaHolder<T>::m_Loaders.erase(ToLower(*i));
}


/////////////////////////////////////////////////////////////
/// Charge un media de type T à partir d'un fichier
///
/// \param Filename : Chemin du fichier
///
/// \return Pointeur sur le T chargé
///
////////////////////////////////////////////////////////////
template <class T>
inline T* CMediaManager::LoadMediaFromFile(const CFile& Filename) const
{
    // Recherche du fichier dans les répertoires enregistrés
    CFile MediaPath = FindMedia(Filename);

    // On délègue le boulot au loader approprié
    T* Media = FindLoader<T>(MediaPath).LoadFromFile(MediaPath.Fullname());

    // Loggization du chargement
    Logger::Log() << "[LOAD] New resources loaded : " << MediaPath.Fullname() << "\n";

    return Media;
}


/////////////////////////////////////////////////////////////
/// Sauvegarde un T dans un fichier
///
/// \param Object :   Pointeur sur l'objet à sauvegarder
/// \param Filename : Nom du fichier à créer
///
////////////////////////////////////////////////////////////
template <class T>
inline void CMediaManager::SaveMediaToFile(const T* Object, const CFile& Filename) const
{
    // On délègue le boulot au loader approprié
    FindLoader<T>(Filename).SaveToFile(Object, Filename.Fullname());

    // Loggization de la sauvegarde
    Logger::Log() << "[SAVE] resources saved : " << Filename.Fullname() << "\n";
}


/////////////////////////////////////////////////////////////
/// Cherche le loader correspondant à un fichier donné
///
/// \param Filename : Fichier
///
/// \return Référence sur le loader, exception si non-trouvé
///
////////////////////////////////////////////////////////////
template <class T>
inline ILoader<T>& CMediaManager::FindLoader(const CFile& Filename) const
{
	Logger::Log() << "[DEBUG] Number extension supported : " << CMediaHolder<T>::m_Loaders.size() << "\n";
    // Recherche de l'extension dans la map de loaders
    typename CMediaHolder<T>::TLoadersMap::const_iterator It = CMediaHolder<T>::m_Loaders.find(ToLower(Filename.Extension()));

    // Si l'extension du fichier se trouve parmi celles reconnues on renvoie le loader associé, sinon on lance une exception
    if ((It != CMediaHolder<T>::m_Loaders.end()) && It->second)
        return *It->second;

	It =  CMediaHolder<T>::m_Loaders.begin();
	Logger::Log() << "[INFO] Extension supported : \n"; 
	while(It !=  CMediaHolder<T>::m_Loaders.end())
	{
		Logger::Log() << "   * "<< It->first << "\n";
		++It;
	}

    throw CLoadingFailed(Filename.Fullname(), "Aucun loader ne prend en charge ce format de fichier");
}
