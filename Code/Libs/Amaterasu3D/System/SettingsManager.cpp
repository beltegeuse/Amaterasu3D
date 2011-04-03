//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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
// E-mail : adrien.gruson@gmail.com
//==========================================================
#include "SettingsManager.h"
#include <System/MediaManager.h>
#include <Graphics/MatrixManagement.h>
#include <tinyxml.h>
#include <Logger/Logger.h>
#include <Debug/Exceptions.h>
#include <Logger/LoggerFile.h>
#include <Logger/LoggerDebug.h>
#include <TinyXMLHelper.h>

SINGLETON_IMPL(CSettingsManager)

CSettingsManager::CSettingsManager() :
	VerticalSync(false),
	m_NearClipping(1.0),
	m_FarClipping(1000.0),
	m_FOV(70)
{
	// The default values
	m_SizeRenderingWindow = Math::TVector2I(800,600);
}

CSettingsManager::~CSettingsManager()
{
}

// Load the config file
// caution: Need the relative path
void CSettingsManager::LoadFile(const std::string& path)
{
	//XXX: Proteger les appels TinyXML avec le Helper
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
		SetSizeRenderingWindow(Math::TVector2I(x,y));
	}
	// * Data
	TiXmlElement* nodeData = rootConfig->FirstChildElement("Data");
	if(nodeData)
	{
		std::string rootDataDir = std::string(nodeData->Attribute("rootDir"));
		Logger::Log() << "[INFO] SettingsManager : Root data dir : " << rootDataDir << "\n";
		CMediaManager::Instance().AddSearchPathAndChilds(rootDataDir);
	}
	// * Projection
	TiXmlElement* nodeProjection = rootConfig->FirstChildElement("Projection");
	if(nodeProjection)
	{
		float fov, near, far;
		TinyXMLGetAttributeValue(nodeProjection, "fov",&fov);
		TinyXMLGetAttributeValue(nodeProjection, "near", &near);
		TinyXMLGetAttributeValue(nodeProjection, "far", &far);
		Logger::Log() << "[INFO] SettingsManager : Projection Settings : \n";
		Logger::Log() << "     * FOV : " << fov << "\n";
		Logger::Log() << "     * Near : " << near << "\n";
		Logger::Log() << "     * Far : " << far << "\n";
		SetProjection(near, far, fov);
	}
}

// To manage the Size of the rendering window
const Math::TVector2I& CSettingsManager::GetSizeRenderingWindow() const
{
	return m_SizeRenderingWindow;
}

void CSettingsManager::SetSizeRenderingWindow(const Math::TVector2I& newSize)
{
	m_SizeRenderingWindow = newSize;
	UpdateProjectionMatrix();
}

// Projections methods
void CSettingsManager::SetProjection(float near, float far, float fov)
{
	m_NearClipping = near;
	m_FarClipping = far;
	m_FOV = fov;
	UpdateProjectionMatrix();
}

float CSettingsManager::GetNearClipping() const
{
	return m_NearClipping;
}

float CSettingsManager::GetFarClipping() const
{
	return m_FarClipping;
}

float CSettingsManager::GetFOV() const
{
	return m_FOV;
}

/*
 * Private methods
 */
void CSettingsManager::UpdateProjectionMatrix()
{
	CMatrixManager::Instance().SetProjectionMatrix(
			Math::CMatrix4::CreatePerspectiveFOV(m_FOV, m_SizeRenderingWindow.x/(float)m_SizeRenderingWindow.y,
				m_NearClipping, m_FarClipping));
}
