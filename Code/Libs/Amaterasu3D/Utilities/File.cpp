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
#include <Utilities/File.h>
#include <algorithm>
#include <fstream>


/////////////////////////////////////////////////////////////
/// Constructeur à partir d'un std::string
///
/// \param Name : Chemin complet du fichier
///
////////////////////////////////////////////////////////////
CFile::CFile(const std::string& Name) :
m_Name(Name)
{
    std::replace(m_Name.begin(), m_Name.end(), '/', '\\');
}


/////////////////////////////////////////////////////////////
/// Constructeur à partir d'un const char*
///
/// \param Name : Chemin complet du fichier
///
////////////////////////////////////////////////////////////
CFile::CFile(const char* Name) :
m_Name(Name)
{
    std::replace(m_Name.begin(), m_Name.end(), '/', '\\');
}


/////////////////////////////////////////////////////////////
/// Indique si le fichier existe ou non
///
/// \return True si le fichier existe
///
////////////////////////////////////////////////////////////
bool CFile::Exists() const
{
	std::ifstream File(m_Name.c_str());

    return File.is_open();
}


/////////////////////////////////////////////////////////////
/// Renvoie le nom du fichier avec son chemin complet
///
/// \return Chemin complet du fichier
///
////////////////////////////////////////////////////////////
const std::string& CFile::Fullname() const
{
    return m_Name;
}


/////////////////////////////////////////////////////////////
/// Renvoie le nom du fichier sans son chemin
///
/// \return Nom du fichier
///
////////////////////////////////////////////////////////////
std::string CFile::Filename() const
{
    std::string::size_type Pos = m_Name.find_last_of("\\/");

    if (Pos != std::string::npos)
        return m_Name.substr(Pos + 1, std::string::npos);
    else
        return m_Name;
}


/////////////////////////////////////////////////////////////
/// Renvoie le nom du fichier sans extension ni chemin
///
/// \return Nom du fichier
///
////////////////////////////////////////////////////////////
std::string CFile::ShortFilename() const
{
    return Filename().substr(0, Filename().find_last_of("."));
}


/////////////////////////////////////////////////////////////
/// Renvoie l'extension du fichier
///
/// \return Extension du fichier
///
////////////////////////////////////////////////////////////
std::string CFile::Extension() const
{
    std::string::size_type Pos = m_Name.find_last_of(".");
    if (Pos != std::string::npos)
        return m_Name.substr(Pos + 1, std::string::npos);
    else
        return "";
}

