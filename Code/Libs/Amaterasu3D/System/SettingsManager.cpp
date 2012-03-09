//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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
#include <iostream> //#include <Logger/Logger.h>
#include <Exceptions.h>
//#include <LoggerFile.h>
//#include <LoggerDebug.h>
#include <TinyXMLHelper.h>

namespace ama3D
{
SINGLETON_IMPL(CSettingsManager)

CSettingsManager::CSettingsManager() :
		VerticalSync(false), m_NearClipping(1.0), m_FarClipping(1000.0), m_FOV(
				70)
{
	// The default values
	m_SizeRenderingWindow = glm::ivec2(800, 600);
	m_RootDataDir = "../Donnees";
}

CSettingsManager::~CSettingsManager()
{
}

// Load the config file
// caution: Need the relative path
void CSettingsManager::LoadFile(const std::string& path)
{
	//XXX: Proteger les appels TinyXML avec le Helper
	TiXmlDocument doc(path.c_str());
	if (!doc.LoadFile())
	{
		std::cout << "[ERROR] TinyXML error : " << doc.ErrorDesc() << "\n";
		throw CLoadingFailed(path, "unable to load xml with TinyXML");
	}

	// Get the root
	TiXmlHandle hdl(&doc);
	TiXmlElement *rootConfig = hdl.FirstChild("Config").Element();
	if (!rootConfig)
	{
		throw CLoadingFailed(path,
				"unable to find Config node in the XML file");
	}

	/////////////////
	// Extract Data
	/////////////////
	// * Logger
	TiXmlElement* nodeLogger = rootConfig->FirstChildElement("Logger");
	if (nodeLogger)
	{
		std::string loggerType = std::string(nodeLogger->Attribute("type"));
		if (loggerType == "File")
		{
			std::string fileLog = std::string(nodeLogger->Attribute("file"));
			//Logger::SetLogger(new LoggerFile(fileLog)); // FIXME
			std::cout << "[INFO] SettingsManager : Log to a file : "
					<< fileLog << "\n";
		}
		else if (loggerType == "Debug")
		{
			//Logger::SetLogger(new LoggerDebug); // FIXME
		}
		else
		{
			throw CLoadingFailed(path, "unknow logger type in the XML file");
		}
	}
	// * Resolution
	TiXmlElement* nodeResolution = rootConfig->FirstChildElement("Resolution");
	if (nodeResolution)
	{
		int x, y;
		nodeResolution->Attribute("x", &x);
		nodeResolution->Attribute("y", &y);
		std::cout << "[INFO] SettingsManager : Resolution = " << x << "x"
				<< y << "\n";
		SetSizeRenderingWindow(glm::ivec2(x, y));
	}
	// * Data
	TiXmlElement* nodeData = rootConfig->FirstChildElement("Data");
	if (nodeData)
	{
		std::string rootDataDir = std::string(nodeData->Attribute("rootDir"));
		std::cout << "[INFO] SettingsManager : Root data dir : "
				<< rootDataDir << "\n";
		m_RootDataDir = rootDataDir;
		CMediaManager::Instance().AddSearchPathAndChilds(rootDataDir);
	}
	// * Projection
	TiXmlElement* nodeProjection = rootConfig->FirstChildElement("Projection");
	if (nodeProjection)
	{
		float fov, near, far;
		TinyXMLGetAttributeValue(nodeProjection, "fov", &fov);
		TinyXMLGetAttributeValue(nodeProjection, "near", &near);
		TinyXMLGetAttributeValue(nodeProjection, "far", &far);
		std::cout << "[INFO] SettingsManager : Projection Settings : \n";
		std::cout << "     * FOV : " << fov << "\n";
		std::cout << "     * Near : " << near << "\n";
		std::cout << "     * Far : " << far << "\n";
		SetProjection(near, far, fov);
	}
}

// To manage the Size of the rendering window
const glm::ivec2& CSettingsManager::GetSizeRenderingWindow() const
{
	return m_SizeRenderingWindow;
}

void CSettingsManager::SetSizeRenderingWindow(const glm::ivec2& newSize)
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

const std::string& CSettingsManager::GetRootDir() const
{
	return m_RootDataDir;
}

/*
 * Private methods
 */
void CSettingsManager::UpdateProjectionMatrix()
{
	CMatrixManager::Instance().SetProjectionMatrix(
			glm::perspectiveFov(m_FOV,
					(float)m_SizeRenderingWindow.x,(float) m_SizeRenderingWindow.y,
					m_NearClipping, m_FarClipping));
}
}
