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

#ifndef EXCEPTION_H
#define EXCEPTION_H

//==========================================================
// En-tetes
//==========================================================
#include <exception>
#include <string>

    ////////////////////////////////////////////////////////////
    /// Classe de base pour les exceptions
    ////////////////////////////////////////////////////////////
    class CException : public std::exception
    {
    public :

        //----------------------------------------------------------
        // Constructeur
        //----------------------------------------------------------
        CException(const std::string& Message = "");

        //----------------------------------------------------------
        // Destructeur
        //----------------------------------------------------------
        virtual ~CException() throw();

        //----------------------------------------------------------
        // Renvoie le message associ� � l'exception
        //----------------------------------------------------------
        virtual const char* what() const throw();

    protected :

        //----------------------------------------------------------
        // Donnees membres
        //----------------------------------------------------------
        std::string m_Message; ///< Message d�crivant l'exception
    };


    ////////////////////////////////////////////////////////////
    /// Exception lanc�e si une condition n'est pas v�rifi�e
    ////////////////////////////////////////////////////////////
    struct CAssertException : public CException
    {
        CAssertException(const std::string& File, int Line, const std::string& Message);
    };
    #ifdef _DEBUG
    #   define Assert(condition) if (!(condition)) throw CAssertException(__FILE__, __LINE__, "Condition non satisfaite\n" #condition)
    #else
        inline void DoNothing(bool) {}
    #   define Assert(condition) DoNothing(!(condition))
    #endif

//==========================================================
// Exception lancee lors d'erreur de chargement de fichiers
//==========================================================
struct CLoadingFailed : public CException
{
	CLoadingFailed(const std::string& File, const std::string& Message);
};


#endif // EXCEPTION_H
