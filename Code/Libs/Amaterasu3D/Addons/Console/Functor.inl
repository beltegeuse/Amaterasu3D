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


////////////////////////////////////////////////////////////
/// Construit le foncteur à partir d'une fonction
///
/// \param Func : Pointeur sur l'objet stockant la fonction
///
////////////////////////////////////////////////////////////
inline CFunctor::CFunctor(IFunction* Func) :
m_Function(Func)
{

}


////////////////////////////////////////////////////////////
/// Effectue l'appel à la fonction
///
/// \param Params : Paramètres à passer sous forme de chaîne
///
/// \return Retour de la fonction sous forme de chaîne
///
////////////////////////////////////////////////////////////
inline std::string CFunctor::operator ()(const std::string& Params) const
{
    if (!m_Function)
        throw CException("Tentative d'effectuer un appel à une fonction nulle via un foncteur");

    return m_Function->Execute(Params);
}


////////////////////////////////////////////////////////////
/// Structure permettant d'accéder au type "primitif" d'un
/// paramètre (sans les éventuels références et / ou const)
////////////////////////////////////////////////////////////
template <typename T> struct Base {typedef T Type;};
template <typename T> struct Base<T&> {typedef T Type;};
template <typename T> struct Base<const T> {typedef T Type;};
template <typename T> struct Base<const T&> {typedef T Type;};


////////////////////////////////////////////////////////////
/// Structure utilisée pour effectuer les appels de fonction libres.
/// Son rôle est surtout de faire la différence entre les
/// fonctions ne renvoyant rien et les autres, sans avoir
/// à spécialiser tous les CFunctionX
////////////////////////////////////////////////////////////
template <typename Ret>
struct CallFun
{
    template <typename FuncType>
    static std::string Do(FuncType Func)
    {
        return CStringBuilder(Func());
    }

    template <typename FuncType, typename Arg1>
    static std::string Do(FuncType Func, Arg1 a1)
    {
        return CStringBuilder(Func(a1));
    }

    template <typename FuncType, typename Arg1, typename Arg2>
    static std::string Do(FuncType Func, Arg1 a1, Arg2 a2)
    {
        return CStringBuilder(Func(a1, a2));
    }
};

////////////////////////////////////////////////////////////
/// Spécialisation pour les fonctions libres ne renvoyant rien
////////////////////////////////////////////////////////////
template <>
struct CallFun<void>
{
    template <typename FuncType>
    static std::string Do(FuncType Func)
    {
        Func();
        return "";
    }

    template <typename FuncType, typename Arg1>
    static std::string Do(FuncType Func, Arg1 a1)
    {
        Func(a1);
        return "";
    }

    template <typename FuncType, typename Arg1, typename Arg2>
    static std::string Do(FuncType Func, Arg1 a1, Arg2 a2)
    {
        Func(a1, a2);
        return "";
    }
};

////////////////////////////////////////////////////////////
/// Structure utilisée pour effectuer les appels de fonction membres.
/// Son rôle est surtout de faire la différence entre les
/// fonctions ne renvoyant rien et les autres, sans avoir
/// à spécialiser tous les CFunctionMemX
////////////////////////////////////////////////////////////
template <typename Ret, typename ParamType>
struct CallMemFun
{
    template <typename FuncType>
    static std::string Do(ParamType Obj, FuncType Func)
    {
        return CStringBuilder((Obj.*Func)());
    }

    template <typename FuncType, typename Arg1>
    static std::string Do(ParamType Obj, FuncType Func, Arg1 a1)
    {
        return CStringBuilder((Obj.*Func)(a1));
    }

    template <typename FuncType, typename Arg1, typename Arg2>
    static std::string Do(ParamType Obj, FuncType Func, Arg1 a1, Arg2 a2)
    {
        return CStringBuilder((Obj.*Func)(a1, a2));
    }
};


////////////////////////////////////////////////////////////
/// Spécialisation pour les fonctions membres ne renvoyant rien
////////////////////////////////////////////////////////////
template <typename ParamType>
struct CallMemFun<void, ParamType>
{
    template <typename FuncType>
    static std::string Do(ParamType Obj, FuncType Func)
    {
        (Obj.*Func)();
        return "";
    }

    template <typename FuncType, typename Arg1>
    static std::string Do(ParamType Obj, FuncType Func, Arg1 a1)
    {
        (Obj.*Func)(a1);
        return "";
    }

    template <typename FuncType, typename Arg1, typename Arg2>
    static std::string Do(ParamType Obj, FuncType Func, Arg1 a1, Arg2 a2)
    {
        (Obj.*Func)(a1, a2);
        return "";
    }
};


////////////////////////////////////////////////////////////
/// Classe encapsulant les fonctions libres sans paramètre
////////////////////////////////////////////////////////////
template <typename Ret>
class CFunction0 : public IFunction
{
    virtual std::string Execute(const std::string& Params)
    {
        CStringExtractor(Params.c_str()).ThrowIfEOF();

        return CallFun<Ret>::Do(m_Func);
    }

    typedef Ret (*TFuncType)();
    TFuncType m_Func;

public :

    CFunction0(TFuncType Func) : m_Func(Func) {}
};


////////////////////////////////////////////////////////////
/// Classe encapsulant les fonctions libres à 1 paramètre
////////////////////////////////////////////////////////////
template <typename Ret, typename Arg1>
class CFunction1 : public IFunction
{
private :

    virtual std::string Execute(const std::string& Params)
    {
        typename Base<Arg1>::Type a1;
        CStringExtractor(Params.c_str())(a1).ThrowIfEOF();

        return CallFun<Ret>::Do(m_Func, a1);
    }

    typedef Ret (*TFuncType)(Arg1);
    TFuncType m_Func;

public :

    CFunction1(TFuncType Func) : m_Func(Func) {}
};


////////////////////////////////////////////////////////////
/// Classe encapsulant les fonctions libres à 2 paramètres
////////////////////////////////////////////////////////////
template <typename Ret, typename Arg1, typename Arg2>
class CFunction2 : public IFunction
{
private :

    virtual std::string Execute(const std::string& Params)
    {
        typename Base<Arg1>::Type a1;
        typename Base<Arg2>::Type a2;
        CStringExtractor(Params.c_str())(a1)(a2).ThrowIfEOF();

        return CallFun<Ret>::Do(m_Func, a1, a2);
    }

    typedef Ret (*TFuncType)(Arg1, Arg2);
    TFuncType m_Func;

public :

    CFunction2(TFuncType Func) : m_Func(Func) {}
};


////////////////////////////////////////////////////////////
/// Classe encapsulant les fonctions membres sans paramètre
////////////////////////////////////////////////////////////
template <typename ParamType, typename FuncType, typename Ret>
class CFunctionMem0 : public IFunction
{
private :

    virtual std::string Execute(const std::string& Params)
    {
        CStringExtractor(Params.c_str()).ThrowIfEOF();

        return CallMemFun<Ret, ParamType>::Do(m_Obj, m_Func);
    }

    FuncType  m_Func;
    ParamType m_Obj;

public :

    CFunctionMem0(FuncType Func, ParamType Obj) : m_Func(Func), m_Obj(Obj) {}
};


////////////////////////////////////////////////////////////
/// Classe encapsulant les fonctions membres à 1 paramètre
////////////////////////////////////////////////////////////
template <typename ParamType, typename FuncType, typename Ret, typename Arg1>
class CFunctionMem1 : public IFunction
{
private :

    virtual std::string Execute(const std::string& Params)
    {
        typename Base<Arg1>::Type a1;
        CStringExtractor(Params.c_str())(a1).ThrowIfEOF();

        return CallMemFun<Ret, ParamType>::Do(m_Obj, m_Func, a1);
    }

    FuncType  m_Func;
    ParamType m_Obj;

public :

    CFunctionMem1(FuncType Func, ParamType Obj) : m_Func(Func), m_Obj(Obj) {}
};


////////////////////////////////////////////////////////////
/// Classe encapsulant les fonctions membres à 2 paramètres
////////////////////////////////////////////////////////////
template <typename ParamType, typename FuncType, typename Ret, typename Arg1, typename Arg2>
class CFunctionMem2 : public IFunction
{
private :

    virtual std::string Execute(const std::string& Params)
    {
        typename Base<Arg1>::Type a1;
        typename Base<Arg2>::Type a2;
        CStringExtractor(Params.c_str())(a1)(a2).ThrowIfEOF();

        return CallMemFun<Ret, ParamType>::Do(m_Obj, m_Func, a1, a2);
    }

    FuncType  m_Func;
    ParamType m_Obj;

public :

    CFunctionMem2(FuncType Func, ParamType Obj) : m_Func(Func), m_Obj(Obj) {}
};


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre non-const sans paramètre
/// dans un foncteur -- stocke une copie de l'instance
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction membre
///
////////////////////////////////////////////////////////////
template <typename Class, typename Ret>
inline CFunctor BindCopy(Ret (Class::*Func)(), Class Obj)
{
    return new CFunctionMem0<Class, Ret (Class::*)(), Ret>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre non-const à 1 paramètre
/// dans un foncteur -- stocke une copie de l'instance
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction membre
///
////////////////////////////////////////////////////////////
template <typename Class, typename Ret, typename Arg1>
inline CFunctor BindCopy(Ret (Class::*Func)(Arg1), Class Obj)
{
    return new CFunctionMem1<Class, Ret (Class::*)(Arg1), Ret, Arg1>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre non-const à 2 paramètres
/// dans un foncteur -- stocke une copie de l'instance
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction membre
///
////////////////////////////////////////////////////////////
template <typename Class, typename Ret, typename Arg1, typename Arg2>
inline CFunctor BindCopy(Ret (Class::*Func)(Arg1, Arg2), Class Obj)
{
    return new CFunctionMem2<Class, Ret (Class::*)(Arg1, Arg2), Ret, Arg1, Arg2>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre const sans paramètre
/// dans un foncteur -- stocke une copie de l'instance
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction membre
///
////////////////////////////////////////////////////////////
template <typename Class, typename Ret>
inline CFunctor BindCopy(Ret (Class::*Func)() const, Class Obj)
{
    return new CFunctionMem0<Class, Ret (Class::*)() const, Ret>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre const à 1 paramètre
/// dans un foncteur -- stocke une copie de l'instance
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction membre
///
////////////////////////////////////////////////////////////
template <typename Class, typename Ret, typename Arg1>
inline CFunctor BindCopy(Ret (Class::*Func)(Arg1) const, Class Obj)
{
    return new CFunctionMem1<Class, Ret (Class::*)(Arg1) const, Ret, Arg1>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre const à 2 paramètres
/// dans un foncteur -- stocke une copie de l'instance
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction membre
///
////////////////////////////////////////////////////////////
template <typename Class, typename Ret, typename Arg1, typename Arg2>
inline CFunctor BindCopy(Ret (Class::*Func)(Arg1, Arg2) const, Class Obj)
{
    return new CFunctionMem2<Class, Ret (Class::*)(Arg1, Arg2) const, Ret, Arg1, Arg2>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule un objet-fonction dans un foncteur
///
/// \param Obj : Objet-fonction à encapsuler
///
/// \return Foncteur contenant l'objet-fonction
///
////////////////////////////////////////////////////////////
template <typename T>
inline CFunctor Bind(T Obj)
{
    return BindCopy(&T::operator (), Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction libre dans un foncteur -
/// Surcharge pour les fonctions sans paramètre
///
/// \param Func : Fonction à encapsuler
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Ret>
inline CFunctor Bind(Ret (*Func)())
{
    return new CFunction0<Ret>(Func);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction libre dans un foncteur -
/// Surcharge pour les fonctions à 1 paramètre
///
/// \param Func : Fonction à encapsuler
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Ret, typename Arg1>
inline CFunctor Bind(Ret (*Func)(Arg1))
{
    return new CFunction1<Ret, Arg1>(Func);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction libre dans un foncteur -
/// Surcharge pour les fonctions à 2 paramètres
///
/// \param Func : Fonction à encapsuler
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Ret, typename Arg1, typename Arg2>
inline CFunctor Bind(Ret (*Func)(Arg1, Arg2))
{
    return new CFunction2<Ret, Arg1, Arg2>(Func);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre dans un foncteur -
/// Surcharge pour les fonctions sans paramètre
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Class, typename ParamType, typename Ret>
inline CFunctor Bind(Ret (Class::*Func)(), ParamType& Obj)
{
    return new CFunctionMem0<ParamType&, Ret (Class::*)(), Ret>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre dans un foncteur -
/// Surcharge pour les fonctions à 1 paramètre
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Class, typename ParamType, typename Ret, typename Arg1>
inline CFunctor Bind(Ret (Class::*Func)(Arg1), ParamType& Obj)
{
    return new CFunctionMem1<ParamType&, Ret (Class::*)(Arg1), Ret, Arg1>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre dans un foncteur -
/// Surcharge pour les fonctions à 2 paramètres
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Class, typename ParamType, typename Ret, typename Arg1, typename Arg2>
inline CFunctor Bind(Ret (Class::*Func)(Arg1, Arg2), ParamType& Obj)
{
    return new CFunctionMem2<ParamType&, Ret (Class::*)(Arg1, Arg2), Ret, Arg1, Arg2>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre constante dans un foncteur -
/// Surcharge pour les fonctions sans paramètre
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Class, typename ParamType, typename Ret>
inline CFunctor Bind(Ret (Class::*Func)() const, const ParamType& Obj)
{
    return new CFunctionMem0<const ParamType&, Ret (Class::*)() const, Ret>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre constante dans un foncteur -
/// Surcharge pour les fonctions à 1 paramètre
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Class, typename ParamType, typename Ret, typename Arg1>
inline CFunctor Bind(Ret (Class::*Func)(Arg1) const, const ParamType& Obj)
{
    return new CFunctionMem1<const ParamType&, Ret (Class::*)(Arg1) const, Ret, Arg1>(Func, Obj);
}


////////////////////////////////////////////////////////////
/// Encapsule une fonction membre constante dans un foncteur -
/// Surcharge pour les fonctions à 2 paramètres
///
/// \param Func : Fonction membre à encapsuler
/// \param Obj :  Instance de la classe
///
/// \return Foncteur contenant la fonction
///
////////////////////////////////////////////////////////////
template <typename Class, typename ParamType, typename Ret, typename Arg1, typename Arg2>
inline CFunctor Bind(Ret (Class::*Func)(Arg1, Arg2) const, const ParamType& Obj)
{
    return new CFunctionMem2<const ParamType&, Ret (Class::*)(Arg1, Arg2) const, Ret, Arg1, Arg2>(Func, Obj);
}
