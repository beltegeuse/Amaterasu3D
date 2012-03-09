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
/// \param Extensions : Extensions associees au chargeur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CMediaManager::RegisterLoader(ILoader<T>* Loader, const std::string& Extensions)
{
    // Recuperation des extensions
    std::vector<std::string> Ext;
    Split(Extensions, Ext, " /\\*.,;|-_\t\n'\"");

	std::cout  << "[INFO] Add New Loader ... \n";

    // Ajout des extensions une e une
    CSmartPtr<ILoader<T> > Ptr = Loader;
    for (std::vector<std::string>::iterator i = Ext.begin(); i != Ext.end(); ++i)
	{
		std::cout  << "  * " << ToLower(*i) << " ( " << CMediaHolder<T>::m_Loaders.size() << ")\n";
        CMediaHolder<T>::m_Loaders[ToLower(*i)] = Ptr;
	}
}


////////////////////////////////////////////////////////////
/// Supprime un chargeur
///
/// \param Extensions : Extensions associees au chargeur
///
////////////////////////////////////////////////////////////
template <class T>
inline void CMediaManager::UnregisterLoader(const std::string& Extensions)
{
    // Recuperation des extensions
    std::vector<std::string> Ext;
    Split(Extensions, Ext, " /\\*.,;|-_\t\n'\"");

    // Suppression du chargeur pour chaque extension
    for (std::vector<std::string>::iterator i = Ext.begin(); i != Ext.end(); ++i)
        CMediaHolder<T>::m_Loaders.erase(ToLower(*i));
}


/////////////////////////////////////////////////////////////
/// Charge un media de type T e partir d'un fichier
///
/// \param Filename : Chemin du fichier
///
/// \return Pointeur sur le T charge
///
////////////////////////////////////////////////////////////
template <class T>
inline T* CMediaManager::LoadMediaFromFile(const CFile& Filename) const
{
    // Recherche du fichier dans les repertoires enregistres
    CFile MediaPath = FindMedia(Filename);

    // On delegue le boulot au loader approprie
    T* Media;
    try
    {
    	Media = FindLoader<T>(MediaPath).LoadFromFile(MediaPath.Fullname());
    }
    catch(CException e)
    {
    	std::cout  << "[ERROR] On loading " << MediaPath.Fullname() << " [Reason : " << e.what() << "]\n";
    	throw CException("Unable to load " + MediaPath.Fullname());
    }

    // Loggization du chargement
    std::cout  << "[LOAD] New resources loaded : " << MediaPath.Fullname() << "\n";

    return Media;
}


/////////////////////////////////////////////////////////////
/// Sauvegarde un T dans un fichier
///
/// \param Object :   Pointeur sur l'objet e sauvegarder
/// \param Filename : Nom du fichier e creer
///
////////////////////////////////////////////////////////////
template <class T>
inline void CMediaManager::SaveMediaToFile(const T* Object, const CFile& Filename) const
{
    // On delegue le boulot au loader approprie
    FindLoader<T>(Filename).SaveToFile(Object, Filename.Fullname());

    // Loggization de la sauvegarde
    std::cout  << "[SAVE] resources saved : " << Filename.Fullname() << "\n";
}


/////////////////////////////////////////////////////////////
/// Cherche le loader correspondant e un fichier donne
///
/// \param Filename : Fichier
///
/// \return Reference sur le loader, exception si non-trouve
///
////////////////////////////////////////////////////////////
template <class T>
inline ILoader<T>& CMediaManager::FindLoader(const CFile& Filename) const
{
	std::cout  << "[DEBUG] Number extension supported : " << CMediaHolder<T>::m_Loaders.size() << "\n";
    // Recherche de l'extension dans la map de loaders
    typename CMediaHolder<T>::TLoadersMap::const_iterator It = CMediaHolder<T>::m_Loaders.find(ToLower(Filename.Extension()));

    // Si l'extension du fichier se trouve parmi celles reconnues on renvoie le loader associe, sinon on lance une exception
    if ((It != CMediaHolder<T>::m_Loaders.end()) && It->second)
        return *It->second;

	It =  CMediaHolder<T>::m_Loaders.begin();
	std::cout  << "[INFO] Extension supported : \n"; 
	while(It !=  CMediaHolder<T>::m_Loaders.end())
	{
		std::cout  << "   * "<< It->first << "\n";
		++It;
	}

    throw CLoadingFailed(Filename.Fullname(), "Aucun loader ne prend en charge ce format de fichier");
}
