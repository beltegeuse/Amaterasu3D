#include <Logger/LoggerDebug.h>

#include <iostream>
namespace ama3D
{
LoggerDebug::LoggerDebug()
{
	std::cout << "***** LoggerDebug 0.1v *******\n";
}

LoggerDebug::~LoggerDebug()
{
}

void LoggerDebug::Write(const std::string& Message)
{
	std::cout << Message;
}
}
