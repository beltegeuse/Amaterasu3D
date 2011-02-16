#ifndef LOGGERFILE_H_
#define LOGGERFILE_H_

/*
 * Heritage de Logger.
 */

#include <Logger/Logger.h>

#include <iostream>
#include <fstream>



class LoggerFile : public Logger
{
private:
	/**
	 * Attributs
	 */
	std::ofstream _File;
	//! Definition de la fonction Write.
	virtual void Write(const std::string& Message);
public:
	LoggerFile(const std::string& Filename = "Output.log");
	virtual ~LoggerFile();
};

#endif /* LOGGERFILE_H_ */
