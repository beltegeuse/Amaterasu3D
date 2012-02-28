#include <Logger/LoggerFile.h>

#include <iostream>
namespace ama3D
{
LoggerFile::LoggerFile(const std::string& Filename) :
		_File(Filename.c_str())
{
	if (!_File)
	{
		std::cerr << "[ERREUR] Impossible d'ouvrir le fichier de log : "
				<< Filename << std::endl;
	}
	_File << "***** LoggerFile 0.1v *******\n";
}

LoggerFile::~LoggerFile()
{
	_File << "***** EndLog *******\n";
	_File.close();
}

void LoggerFile::Write(const std::string& Message)
{
	_File << Message;
}

void LoggerFile::Flush()
{
	_File.flush();
}
}
