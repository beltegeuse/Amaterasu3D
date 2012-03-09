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

#include <Exceptions.h>

/////////////////////////////////////////////////////////////
/// Decoupe une chaine de caracteres
///
/// \param String : Chaene e decomposer
/// \param Tokens : Tableau contenant les tokens recuperes
/// \param Delim :  Delimiteurs
///
////////////////////////////////////////////////////////////
inline void Split(const std::string& String, std::vector<std::string>& Tokens, const std::string& Delim)
{
    // Effacement du vector dans lequel on va mettre les resultats
    Tokens.clear();

    // Parcours de la chaene et extraction des tokens un e un
    for (std::string::size_type p1 = 0, p2 = 0; p1 != std::string::npos; )
    {
        p1 = String.find_first_not_of(Delim, p1);
        if (p1 != std::string::npos)
        {
            p2 = String.find_first_of(Delim , p1);
            Tokens.push_back(String.substr(p1, p2 - p1));
            p1 = p2;
        }
    }
}


////////////////////////////////////////////////////////////
/// Renvoie le contenu d'un fichier sous forme de chaene
///
/// \param Filename : Nom du fichier e lire
///
/// \return Contenu du fichier
///
////////////////////////////////////////////////////////////
inline std::string ReadFile(const std::string& Filename)
{
    // Ouverture du fichier
    std::ifstream File(Filename.c_str());
    if (!File)
        throw CLoadingFailed(Filename, "Impossible d'acceder au fichier en lecture");

    // Recuperation du contenu
    std::ostringstream oss;
    oss << File.rdbuf();

    return oss.str();
}


/////////////////////////////////////////////////////////////
/// Renvoie le code de hachage d'une chaene de caracteres
///
/// \param String : Chaene de caracteres
///
/// \return Code de hachage correspondant e la chaene
///
////////////////////////////////////////////////////////////
inline unsigned long StringHash(const std::string& String)
{
    unsigned long Ret = 0;
    for (std::string::const_iterator i = String.begin(); i != String.end(); ++i)
        Ret = 5 * Ret + *i;

    return Ret;
}


/////////////////////////////////////////////////////////////
/// Convertit une chaene de caracteres en minuscule
///
/// \param Text : Chaene d'entree
///
/// \return Chaene convertie
///
////////////////////////////////////////////////////////////
inline std::string ToLower(const std::string& Text)
{
    std::string Ret(Text.size(), ' ');

    // Le cast est necessaire pour faire fonctionner ce code sous gcc
    std::transform(Text.begin(), Text.end(), Ret.begin(), static_cast<int (*)(int)>(std::tolower));

    return Ret;
}


/////////////////////////////////////////////////////////////
/// Convertit une chaene de caracteres en majuscules
///
/// \param Text : Chaene d'entree
///
/// \return Chaene convertie
///
////////////////////////////////////////////////////////////
inline std::string ToUpper(const std::string& Text)
{
    std::string Ret(Text.size(), ' ');

    // Le cast est necessaire pour faire fonctionner ce code sous gcc
    std::transform(Text.begin(), Text.end(), Ret.begin(), static_cast<int (*)(int)>(std::toupper));

    return Ret;
}

inline bool ToBool(const std::string& Text)
{
	std::string lowText = ToLower(Text);
	if(lowText == "true")
		return true;
	else if (lowText == "false")
		return false;
	else
		throw ama3D::CException("Cannot convert "+Text+" to Bool");
}

////////////////////////////////////////////////////////////
/// Constructeur par defaut
///
////////////////////////////////////////////////////////////
inline CStringBuilder::CStringBuilder()
{

}


////////////////////////////////////////////////////////////
/// Construit l'injecteur avec une premiere valeur
///
/// \param Value : Valeur e injecter
///
////////////////////////////////////////////////////////////
template <typename T>
inline CStringBuilder::CStringBuilder(const T& Value)
{
    m_OutStream << Value;
}


////////////////////////////////////////////////////////////
/// Injecte une valeur dans la chaene
///
/// \param Value : Valeur e injecter
///
/// \return Reference sur l'instance de l'injecteur
///
////////////////////////////////////////////////////////////
template <typename T>
inline CStringBuilder& CStringBuilder::operator ()(const T& Value)
{
    m_OutStream << Value;

    return *this;
}


////////////////////////////////////////////////////////////
/// Operateur de conversion implicite en string
///
/// \return Chaene de caractere construite avec les valeurs injectees
///
////////////////////////////////////////////////////////////
inline CStringBuilder::operator std::string()
{
    return m_OutStream.str();
}


////////////////////////////////////////////////////////////
/// Construit l'extracteur e partir d'une chaene
///
/// \param Text : Texte e convertir
///
////////////////////////////////////////////////////////////
inline CStringExtractor::CStringExtractor(const std::string& Text) :
m_InStream(Text)
{

}


////////////////////////////////////////////////////////////
/// Extrait une valeur de type T
///
/// \param Value : Valeur e extraire
///
/// \return Reference sur l'extracteur
///
////////////////////////////////////////////////////////////
template <typename T>
inline CStringExtractor& CStringExtractor::operator ()(T& Value)
{
    if (!(m_InStream >> Value))
    {
        if (m_InStream.eof())
            throw CBadConversion(CStringBuilder("Tentative d'extraire e partir d'une chaene vide"));
        else
            throw CBadConversion(CStringBuilder("Impossible de convertir"));
    }

    return *this;
}


////////////////////////////////////////////////////////////
/// Verifie qu'il n'y a plus rien e extraire
///
////////////////////////////////////////////////////////////
inline void CStringExtractor::ThrowIfEOF()
{
    std::string Left;
    if (std::getline(m_InStream, Left))
        throw CBadConversion("Chaene trop longue, \"" + Left + "\" n'a pas ete extrait");
}
