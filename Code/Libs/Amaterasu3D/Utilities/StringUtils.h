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
// En-tetes
//==========================================================
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <Exceptions.h>

namespace ama3D
{
//----------------------------------------------------------------
// Decoupe une chaine de caracteres
//----------------------------------------------------------------
void Split(const std::string& String, std::vector<std::string>& Tokens,
		const std::string& Delim = " \t\n");

//----------------------------------------------------------------
// Renvoie le contenu d'un fichier sous forme de chaene
//----------------------------------------------------------------
std::string ReadFile(const std::string& Filename);

//----------------------------------------------------------------
// Renvoie le code de hachage d'une chaene de caracteres
//----------------------------------------------------------------
unsigned long StringHash(const std::string& String);

//----------------------------------------------------------------
// Convertit une chaine de caracteres en minuscule
//----------------------------------------------------------------
std::string ToLower(const std::string& Text);

//----------------------------------------------------------------
// Convertit une chaine de caracteres en majuscules
//----------------------------------------------------------------
std::string ToUpper(const std::string& Text);

//----------------------------------------------------------------
// Convertit une chaine de caracteres en bool
//----------------------------------------------------------------
bool ToBool(const std::string& Text);

template <class T>
struct MyTypeTraits
{
   static const char* name;
};

template <class T>
const char* MyTypeTraits<T>::name = "Unknow";

template <>
struct MyTypeTraits<int>
{
   static const char* name;
};

template <>
struct MyTypeTraits<float>
{
   static const char* name;
};

template < typename T >
class CToCustomType
{
public:
	T operator ()(const std::string& str)
	{
		std::istringstream iss(str);
		T obj;
		iss >> std::ws >> obj >> std::ws;
		if(!iss.eof()) throw ama3D::CException("Cannot convert "+str+" to "+MyTypeTraits<T>::name);
		return obj;
	}
};

typedef CToCustomType<int> ToIntType;
typedef CToCustomType<float> ToFloatType;
////////////////////////////////////////////////////////////
/// Template servant e construire les classes singleton
////////////////////////////////////////////////////////////
class CStringBuilder
{
public:

	//----------------------------------------------------------------
	// Constructeur par defaut
	//----------------------------------------------------------------
	CStringBuilder();

	//----------------------------------------------------------------
	// Construit l'injecteur avec une premiere valeur
	//----------------------------------------------------------------
	template<typename T> CStringBuilder(const T& Value);

	//----------------------------------------------------------------
	// Injecte une valeur dans la chaene
	//----------------------------------------------------------------
	template<typename T> CStringBuilder& operator ()(const T& Value);

	//----------------------------------------------------------------
	// Operateur de conversion implicite en string
	//----------------------------------------------------------------
	operator std::string();

private:

	//----------------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------------
	std::ostringstream m_OutStream; ///< Flux-chaene utilise pourl'injection des valeurs
};

////////////////////////////////////////////////////////////
/// Template servant e construire les classes singleton
////////////////////////////////////////////////////////////
class CStringExtractor
{
public:

	//----------------------------------------------------------------
	// Construit l'extracteur e partir d'une chaene
	//----------------------------------------------------------------
	CStringExtractor(const std::string& Text);

	//----------------------------------------------------------------
	// Extrait une valeur de type T
	//----------------------------------------------------------------
	template<typename T> CStringExtractor& operator ()(T& Value);

	//----------------------------------------------------------------
	// Verifie qu'il n'y a plus rien e extraire
	//----------------------------------------------------------------
	void ThrowIfEOF();

private:

	//----------------------------------------------------------------
	// Donnees membres
	//----------------------------------------------------------------
	std::istringstream m_InStream; ///< Flux-chaene utilise pour l'extraction des valeurs
};

#include "StringUtils.inl"

}
#endif // STRINGUTILS_H
