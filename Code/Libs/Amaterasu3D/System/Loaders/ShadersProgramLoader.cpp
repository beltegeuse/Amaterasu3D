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


//==========================================================
// En-têtes
//==========================================================
#include <System/Loaders/ShadersProgramLoader.h>
#include <Logger/Logger.h>
#include <System/MediaManager.h>
#include <tinyxml.h>

////////////////////////////////////////////////////////////
/// Construit le loader
///
/// \param Type : Type des shaders gérés (vertex / pixel)
///
////////////////////////////////////////////////////////////
CShaderProgramLoader::CShaderProgramLoader()
{
}


////////////////////////////////////////////////////////////
/// Destructeur
///
////////////////////////////////////////////////////////////
CShaderProgramLoader::~CShaderProgramLoader()
{
}


////////////////////////////////////////////////////////////
/// Charge un shader à partir d'un fichier
///
/// \param Filename : Chemin du fichier à charger
///
/// \return Pointeur sur le shader chargé
///
////////////////////////////////////////////////////////////
Shader* CShaderProgramLoader::LoadFromFile(const std::string& Filename)
{
	TiXmlDocument doc( Filename.c_str() );
	if(!doc.LoadFile())
	{
		Logger::Log() << "[ERROR] TinyXML error : " <<  doc.ErrorDesc() << "\n";
		throw CLoadingFailed(Filename, "unable to load xml with TinyXML");
	}
	// Get the root
	TiXmlHandle hdl(&doc);
	TiXmlElement *root = hdl.FirstChild("Shader").Element();
	// Problem to find the root
	if(!root)
	{
		throw CLoadingFailed(Filename, "unable to find root (Shader)");
	}
	// Get the shader name and display it
	std::string name = std::string(root->Attribute("name"));
	Logger::Log() << "[INFO] Load shader : " << name << " ... \n";
	// Get the 2 files name
	// * Vertex shader
	TiXmlElement *shadername = root->FirstChildElement("VertexShader");
	if(!shadername)
	{
		throw CLoadingFailed(Filename, "unable to find VertexShader (Shader)");
	}
	std::string vertexShadername = std::string(shadername->Attribute("filename"));
	Logger::Log() << "   * Vertex shader : " << vertexShadername << "\n";
	// * Fragment shader
	shadername = root->FirstChildElement("FragmentShader");
	if(!shadername)
	{
		throw CLoadingFailed(Filename, "unable to find VertexShader (Shader)");
	}
	std::string fragmentShadername = std::string(shadername->Attribute("filename"));
	Logger::Log() << "   * Fragment shader : " << fragmentShadername << "\n";

	//FIXME: Faire appel au ressource manager
	return new Shader(CMediaManager::Instance().LoadMediaFromFile<ShaderUnit>(vertexShadername),
					  CMediaManager::Instance().LoadMediaFromFile<ShaderUnit>(fragmentShadername));
}
