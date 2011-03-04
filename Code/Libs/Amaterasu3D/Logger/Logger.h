#ifndef LOGGER_H_
#define LOGGER_H_

// Std includes
#include <sstream>
#include <string>
#include <iostream>

// CoreEngine Includes
#include <Debug/Exceptions.h>

class Logger
{
private:
	/**
	 * Attributs
	 */
	static Logger* _instance;

	/**
	 * Methode a redefinir dans les classes derivees.
	 * Methode privee
	 * Affichage du message.
	 */
    virtual void Write(const std::string& Message) = 0;

public:
	/**
	 * Constructeurs et destructeurs
	 */
	Logger();
	virtual ~Logger();

	/**
	 * Methode de changement d'instance de Logger.
	 */
	static void SetLogger(Logger* logger);

	/**
	 * Get de l'instance de Logger.
	 */
    static Logger& Log();
    static void Kill();
    /**
     * Definition de l'operateur de flux.
     */
    template <class T>
    Logger& operator <<(const T& ToLog);
};

template <class T>
Logger& Logger::operator <<(const T& ToLog)
{
	std::ostringstream ss;
	ss << ToLog;
	_instance->Write(ss.str());
	return *(_instance);
}


#endif /* LOGGER_H_ */
