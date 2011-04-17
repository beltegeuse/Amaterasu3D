//==========================================================
// En-tetes
//==========================================================
#include "Exceptions.h"
#include <sstream>

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
