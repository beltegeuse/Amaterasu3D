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
// En-tetes
//==========================================================
#include "Exceptions.h"
#include <sstream>


//namespace Yes
//{

/////////////////////////////////////////////////////////////
/// Constructeur
///
/// \param Message : Message d'erreur
///
////////////////////////////////////////////////////////////
CException::CException(const std::string& Message) :
m_Message(Message)
{

}


/////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
CException::~CException() throw()
{

}


/////////////////////////////////////////////////////////////
/// Renvoie le message associ� � l'exception
///
/// \return Pointeur sur le message
///
////////////////////////////////////////////////////////////
const char* CException::what() const throw()
{
    return m_Message.c_str();
}


/////////////////////////////////////////////////////////////
/// Constructeur
///
/// \param File :    Fichier source contenant l'erreur
/// \param Line :    Ligne � laquelle se trouve l'erreur dans le fichier source
/// \param Message : Message d'erreur
///
////////////////////////////////////////////////////////////
CAssertException::CAssertException(const std::string& File, int Line, const std::string& Message) :
CException("")
{
	std::ostringstream oss;
	oss << File << " (" << Line << ") : " << Message << std::endl;
	m_Message = oss.str();
}

////////////////////////////////////////////////////////////
// Constructeur
//
// [in] File :    Fichier
// [in] Message : Message d'erreur
//
////////////////////////////////////////////////////////////
CLoadingFailed::CLoadingFailed(const std::string& File, const std::string& Message)
{
    // Formatage du message d'erreur
    std::ostringstream oss;
    oss << "Erreur dans le chargement de " << File << std::endl << std::endl << Message;

    m_Message = oss.str();
}

//} // namespace Yes
