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
// En-têtes
//==========================================================
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>


//----------------------------------------------------------------
// Découpe une chaine de caractères
//----------------------------------------------------------------
void Split(const std::string& String, std::vector<std::string>& Tokens, const std::string& Delim = " \t\n");

//----------------------------------------------------------------
// Renvoie le contenu d'un fichier sous forme de chaîne
//----------------------------------------------------------------
std::string ReadFile(const std::string& Filename);

//----------------------------------------------------------------
// Renvoie le code de hachage d'une chaîne de caractères
//----------------------------------------------------------------
unsigned long StringHash(const std::string& String);

//----------------------------------------------------------------
// Convertit une chaîne de caractères en minuscule
//----------------------------------------------------------------
std::string ToLower(const std::string& Text);

//----------------------------------------------------------------
// Convertit une chaîne de caractères en majuscules
//----------------------------------------------------------------
std::string ToUpper(const std::string& Text);


////////////////////////////////////////////////////////////
/// Template servant à construire les classes singleton
////////////////////////////////////////////////////////////
class CStringBuilder
{
public :

	//----------------------------------------------------------------
	// Constructeur par défaut
	//----------------------------------------------------------------
	CStringBuilder();

	//----------------------------------------------------------------
	// Construit l'injecteur avec une première valeur
	//----------------------------------------------------------------
	template <typename T> CStringBuilder(const T& Value);

	//----------------------------------------------------------------
	// Injecte une valeur dans la chaîne
	//----------------------------------------------------------------
	template <typename T> CStringBuilder& operator ()(const T& Value);

	//----------------------------------------------------------------
	// Opérateur de conversion implicite en string
	//----------------------------------------------------------------
	operator std::string();

private :

	//----------------------------------------------------------------
	// Données membres
	//----------------------------------------------------------------
	std::ostringstream m_OutStream; ///< Flux-chaîne utilisé pourl'injection des valeurs
};


////////////////////////////////////////////////////////////
/// Template servant à construire les classes singleton
////////////////////////////////////////////////////////////
class CStringExtractor
{
public :

	//----------------------------------------------------------------
	// Construit l'extracteur à partir d'une chaîne
	//----------------------------------------------------------------
	CStringExtractor(const std::string& Text);

	//----------------------------------------------------------------
	// Extrait une valeur de type T
	//----------------------------------------------------------------
	template <typename T> CStringExtractor& operator ()(T& Value);

	//----------------------------------------------------------------
	// Vérifie qu'il n'y a plus rien à extraire
	//----------------------------------------------------------------
	void ThrowIfEOF();

private :

	//----------------------------------------------------------------
	// Données membres
	//----------------------------------------------------------------
	std::istringstream m_InStream; ///< Flux-chaîne utilisé pour l'extraction des valeurs
};

#include "StringUtils.inl"



#endif // STRINGUTILS_H
