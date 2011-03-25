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
// En-têtes
//==========================================================
#include <System/ResourceManager.h>
#include <Logger/Logger.h>
#include <Debug/Exceptions.h>

////////////////////////////////////////////////////////////
// Implémentation des méthodes du singleton
////////////////////////////////////////////////////////////
SINGLETON_IMPL(CResourceManager)


/////////////////////////////////////////////////////////////
/// Constructeur par défaut
///
////////////////////////////////////////////////////////////
CResourceManager::CResourceManager()
{

}


/////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
CResourceManager::~CResourceManager()
{
    // S'il reste des ressources dans la liste, on le signale
    if (!m_Resources.empty())
    {
        Logger::Log() << "** warning ** leak resources :\n";

        for (TResourcesMap::const_iterator i = m_Resources.begin(); i != m_Resources.end(); ++i)
        {
            Logger::Log() << " - " << i->second->GetName() << "\n";
        }
    }
}


/////////////////////////////////////////////////////////////
/// Ajoute une ressource
///
/// \param Name :     Nom associé à la ressource
/// \param Resource : Pointeur sur la ressource à ajouter
///
////////////////////////////////////////////////////////////
void CResourceManager::Add(const std::string& Name, IResource* Resource)
{
    Assert(Resource != NULL);

    // Si la ressource avait déjà été chargée, on le signale
    if (m_Resources.find(Name) != m_Resources.end())
        Logger::Log() << Name << " : ressource déjà chargée !\n";

    // Ajout de la ressource à la liste
    m_Resources[Name] = Resource;
    Resource->m_Name = Name;
}


/////////////////////////////////////////////////////////////
/// Retire une ressource
///
/// \param Name : Nom de la ressource à retirer
///
////////////////////////////////////////////////////////////
void CResourceManager::Remove(const std::string& Name)
{
    // Recherche de la ressource dans la table
    TResourcesMap::iterator It = m_Resources.find(Name);

    // Si la ressource n'avait pas été chargée, on le signale
    if (It == m_Resources.end())
        Logger::Log() << Name << " : ressource détruite mais non-chargée !\n";

    // Retrait de la ressource de la liste
    m_Resources.erase(It);
}
