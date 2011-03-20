#include "SettingsManager.h"
#include <System/MediaManager.h>
#include <tinyxml.h>
#include <Logger/Logger.h>
#include <Debug/Exceptions.h>
#include <Logger/LoggerFile.h>
#include <Logger/LoggerDebug.h>

SINGLETON_IMPL(SettingsManager)

SettingsManager::SettingsManager()
{
}

SettingsManager::~SettingsManager()
{
	// The default values
	m_SizeRenderingWindow = Math::TVector2I(800,600);
}

// Load the config file
// caution: Need the relative path
void SettingsManager::LoadFile(const std::string& path)
{
	TiXmlDocument doc( path.c_str() );
	if(!doc.LoadFile())
	{
		Logger::Log() << "[ERROR] TinyXML error : " <<  doc.ErrorDesc() << "\n";
		throw CLoadingFailed(path, "unable to load xml with TinyXML");
	}

	// Get the root
	TiXmlHandle hdl(&doc);
	TiXmlElement *rootConfig = hdl.FirstChild("Config").Element();
	if(!rootConfig)
	{
		throw CLoadingFailed(path, "unable to find Config node in the XML file");
	}

	/////////////////
	// Extract Data
	/////////////////
	// * Logger
	TiXmlElement* nodeLogger = rootConfig->FirstChildElement("Logger");
	if(nodeLogger)
	{
		std::string loggerType = std::string(nodeLogger->Attribute("type"));
		if(loggerType == "File")
		{
			std::string fileLog = std::string(nodeLogger->Attribute("file"));
			Logger::SetLogger(new LoggerFile(fileLog));
			Logger::Log() << "[INFO] SettingsManager : Log to a file : " << fileLog << "\n";
		}
		else if(loggerType == "Debug")
		{
			Logger::SetLogger(new LoggerDebug);
		}
		else
		{
			throw CLoadingFailed(path, "unknow logger type in the XML file");
		}
	}
	// * Resolution
	TiXmlElement* nodeResolution = rootConfig->FirstChildElement("Resolution");
	if(nodeResolution)
	{
		int x,y;
		nodeResolution->Attribute("x",&x);
		nodeResolution->Attribute("y",&y);
		Logger::Log() << "[INFO] SettingsManager : Resolution = " << x << "x" << y << "\n";
		m_SizeRenderingWindow = Math::TVector2I(x,y);
	}
	// * Data
	TiXmlElement* nodeData = rootConfig->FirstChildElement("Data");
	if(nodeData)
	{
		std::string rootDataDir = std::string(nodeData->Attribute("rootDir"));
		Logger::Log() << "[INFO] SettingsManager : Root data dir : " << rootDataDir << "\n";
		CMediaManager::Instance().AddSearchPath(rootDataDir);
	}
}

// To manage the Size of the rendering window
const Math::TVector2I& SettingsManager::GetSizeRenderingWindow() const
{
	return m_SizeRenderingWindow;
}

void SettingsManager::SetSizeRenderingWindow(const Math::TVector2I& newSize)
{
	m_SizeRenderingWindow = newSize;
}
