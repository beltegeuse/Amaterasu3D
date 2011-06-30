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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

//==========================================================
// En-t�tes
//==========================================================
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

//----------------------------------------------------------------
// D�coupe une chaine de caract�res
//----------------------------------------------------------------
void Split(const std::string& String, std::vector<std::string>& Tokens,
		const std::string& Delim = " \t\n");

//----------------------------------------------------------------
// Renvoie le contenu d'un fichier sous forme de cha�ne
//----------------------------------------------------------------
std::string ReadFile(const std::string& Filename);

//----------------------------------------------------------------
// Renvoie le code de hachage d'une cha�ne de caract�res
//----------------------------------------------------------------
unsigned long StringHash(const std::string& String);

//----------------------------------------------------------------
// Convertit une cha�ne de caract�res en minuscule
//----------------------------------------------------------------
std::string ToLower(const std::string& Text);

//----------------------------------------------------------------
// Convertit une cha�ne de caract�res en majuscules
//----------------------------------------------------------------
std::string ToUpper(const std::string& Text);

////////////////////////////////////////////////////////////
/// Template servant � construire les classes singleton
////////////////////////////////////////////////////////////
class CStringBuilder
{
public:

	//----------------------------------------------------------------
	// Constructeur par d�faut
	//----------------------------------------------------------------
	CStringBuilder();

	//----------------------------------------------------------------
	// Construit l'injecteur avec une premi�re valeur
	//----------------------------------------------------------------
	template<typename T> CStringBuilder(const T& Value);

	//----------------------------------------------------------------
	// Injecte une valeur dans la cha�ne
	//----------------------------------------------------------------
	template<typename T> CStringBuilder& operator ()(const T& Value);

	//----------------------------------------------------------------
	// Op�rateur de conversion implicite en string
	//----------------------------------------------------------------
	operator std::string();

private:

	//----------------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------------
	std::ostringstream m_OutStream; ///< Flux-cha�ne utilis� pourl'injection des valeurs
};

////////////////////////////////////////////////////////////
/// Template servant � construire les classes singleton
////////////////////////////////////////////////////////////
class CStringExtractor
{
public:

	//----------------------------------------------------------------
	// Construit l'extracteur � partir d'une cha�ne
	//----------------------------------------------------------------
	CStringExtractor(const std::string& Text);

	//----------------------------------------------------------------
	// Extrait une valeur de type T
	//----------------------------------------------------------------
	template<typename T> CStringExtractor& operator ()(T& Value);

	//----------------------------------------------------------------
	// V�rifie qu'il n'y a plus rien � extraire
	//----------------------------------------------------------------
	void ThrowIfEOF();

private:

	//----------------------------------------------------------------
	// Donn�es membres
	//----------------------------------------------------------------
	std::istringstream m_InStream; ///< Flux-cha�ne utilis� pour l'extraction des valeurs
};

#include "StringUtils.inl"

#endif // STRINGUTILS_H
