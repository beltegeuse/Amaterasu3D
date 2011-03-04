#ifndef SINGLETON
#define SINGLETON

#include <iostream>

/// Script repris de : http://www.siteduzero.com/tutoriel-3-32923-la-classe-singleton.html
/// Ecrit par : Davidbrcz


template<typename T> class CSingleton
{
public:

    /*
      Cette fonction retourne l'unique instance de l'objet
    */
    static T* Instance(void)
    {
        if(singleton==0)
	    {
		singleton = new T;
	    }
        return singleton;
    }

    /*
      Cette fonction tue l'unique instance de l'objet
    */
    static void Kill()
    {
        if(singleton!=0)
	    {
		delete singleton ;
		singleton=0;
	    }
    }

protected:

    CSingleton()
    {
    }

    virtual ~CSingleton()
    {
    }

    static T *singleton;
};

template <typename T> T* CSingleton<T>::singleton = 0;

#endif



