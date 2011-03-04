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
#include <Debug/Exceptions.h>

/////////////////////////////////////////////////////////////
/// Découpe une chaine de caractères
///
/// \param String : Chaîne à décomposer
/// \param Tokens : Tableau contenant les tokens récupérés
/// \param Delim :  Délimiteurs
///
////////////////////////////////////////////////////////////
inline void Split(const std::string& String, std::vector<std::string>& Tokens, const std::string& Delim)
{
    // Effacement du vector dans lequel on va mettre les résultats
    Tokens.clear();

    // Parcours de la chaîne et extraction des tokens un à un
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
/// Renvoie le contenu d'un fichier sous forme de chaîne
///
/// \param Filename : Nom du fichier à lire
///
/// \return Contenu du fichier
///
////////////////////////////////////////////////////////////
inline std::string ReadFile(const std::string& Filename)
{
    // Ouverture du fichier
    std::ifstream File(Filename.c_str());
    if (!File)
        throw CLoadingFailed(Filename, "Impossible d'accéder au fichier en lecture");

    // Récupération du contenu
    std::ostringstream oss;
    oss << File.rdbuf();

    return oss.str();
}


/////////////////////////////////////////////////////////////
/// Renvoie le code de hachage d'une chaîne de caractères
///
/// \param String : Chaîne de caractères
///
/// \return Code de hachage correspondant à la chaîne
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
/// Convertit une chaîne de caractères en minuscule
///
/// \param Text : Chaîne d'entrée
///
/// \return Chaîne convertie
///
////////////////////////////////////////////////////////////
inline std::string ToLower(const std::string& Text)
{
    std::string Ret(Text.size(), ' ');

    // Le cast est nécessaire pour faire fonctionner ce code sous gcc
    std::transform(Text.begin(), Text.end(), Ret.begin(), static_cast<int (*)(int)>(std::tolower));

    return Ret;
}


/////////////////////////////////////////////////////////////
/// Convertit une chaîne de caractères en majuscules
///
/// \param Text : Chaîne d'entrée
///
/// \return Chaîne convertie
///
////////////////////////////////////////////////////////////
inline std::string ToUpper(const std::string& Text)
{
    std::string Ret(Text.size(), ' ');

    // Le cast est nécessaire pour faire fonctionner ce code sous gcc
    std::transform(Text.begin(), Text.end(), Ret.begin(), static_cast<int (*)(int)>(std::toupper));

    return Ret;
}


////////////////////////////////////////////////////////////
/// Constructeur par défaut
///
////////////////////////////////////////////////////////////
inline CStringBuilder::CStringBuilder()
{

}


////////////////////////////////////////////////////////////
/// Construit l'injecteur avec une première valeur
///
/// \param Value : Valeur à injecter
///
////////////////////////////////////////////////////////////
template <typename T>
inline CStringBuilder::CStringBuilder(const T& Value)
{
    m_OutStream << Value;
}


////////////////////////////////////////////////////////////
/// Injecte une valeur dans la chaîne
///
/// \param Value : Valeur à injecter
///
/// \return Référence sur l'instance de l'injecteur
///
////////////////////////////////////////////////////////////
template <typename T>
inline CStringBuilder& CStringBuilder::operator ()(const T& Value)
{
    m_OutStream << Value;

    return *this;
}


////////////////////////////////////////////////////////////
/// Opérateur de conversion implicite en string
///
/// \return Chaîne de caractère construite avec les valeurs injectées
///
////////////////////////////////////////////////////////////
inline CStringBuilder::operator std::string()
{
    return m_OutStream.str();
}


////////////////////////////////////////////////////////////
/// Construit l'extracteur à partir d'une chaîne
///
/// \param Text : Texte à convertir
///
////////////////////////////////////////////////////////////
inline CStringExtractor::CStringExtractor(const std::string& Text) :
m_InStream(Text)
{

}


////////////////////////////////////////////////////////////
/// Extrait une valeur de type T
///
/// \param Value : Valeur à extraire
///
/// \return Référence sur l'extracteur
///
////////////////////////////////////////////////////////////
template <typename T>
inline CStringExtractor& CStringExtractor::operator ()(T& Value)
{
    if (!(m_InStream >> Value))
    {
        if (m_InStream.eof())
            throw CBadConversion(CStringBuilder("Tentative d'extraire à partir d'une chaîne vide"));
        else
            throw CBadConversion(CStringBuilder("Impossible de convertir"));
    }

    return *this;
}


////////////////////////////////////////////////////////////
/// Vérifie qu'il n'y a plus rien à extraire
///
////////////////////////////////////////////////////////////
inline void CStringExtractor::ThrowIfEOF()
{
    std::string Left;
    if (std::getline(m_InStream, Left))
        throw CBadConversion("Chaîne trop longue, \"" + Left + "\" n'a pas été extrait");
}
