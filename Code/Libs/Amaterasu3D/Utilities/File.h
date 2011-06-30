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

#ifndef FILE_H
#define FILE_H

//==========================================================
// En-t�tes
//==========================================================
#include <string>

////////////////////////////////////////////////////////////
/// Classe facilitant la manipulation des noms de fichiers
////////////////////////////////////////////////////////////
class CFile
{
public:

	//----------------------------------------------------------
	// Constructeur � partir d'un std::string
	//----------------------------------------------------------
	CFile(const std::string& Name = "unknown");

	//----------------------------------------------------------
	// Constructeur � partir d'un const char*
	//----------------------------------------------------------
	CFile(const char* Name);

	//----------------------------------------------------------
	// Indique si le fichier existe ou non
	//----------------------------------------------------------
	bool Exists() const;

	//----------------------------------------------------------
	// Renvoie le nom du fichier avec son chemin complet
	//----------------------------------------------------------
	const std::string& Fullname() const;

	//----------------------------------------------------------
	// Renvoie le nom du fichier sans son chemin
	//----------------------------------------------------------
	std::string Filename() const;

	//----------------------------------------------------------
	// Renvoie le nom du fichier sans extension ni chemin
	//----------------------------------------------------------
	std::string ShortFilename() const;

	//----------------------------------------------------------
	// Renvoie l'extension du fichier
	//----------------------------------------------------------
	std::string Extension() const;

private:

	//----------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------
	std::string m_Name; ///< Chemin complet du fichier
};

#endif // FILE_H
