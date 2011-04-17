
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


#endif // EXCEPTION_H
