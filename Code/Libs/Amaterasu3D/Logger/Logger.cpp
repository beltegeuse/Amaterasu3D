#include <iostream>

#include <Logger/Logger.h>

// Execption
CLoggerException::CLoggerException(const std::string& Message) :
	CException("[Logger] : " + Message)
{
}


Logger* Logger::_instance = NULL;

Logger::Logger()
{
}

Logger::~Logger()
{
}


void Logger::SetLogger(Logger* logger)
{
	if(_instance != NULL)
	{
		delete _instance;
		_instance = NULL;
	}
	_instance = logger;
}

Logger& Logger::Log()
{
	if(_instance == NULL)
	{
		 throw CLoggerException("Undefined Logger ! Please Use the following fonction : SetLogger(Logger* logger).");
	}
	return *(_instance);
}

void Logger::Kill()
{
	if(_instance != NULL)
	{
		delete _instance;
		_instance = NULL;
	}
}


