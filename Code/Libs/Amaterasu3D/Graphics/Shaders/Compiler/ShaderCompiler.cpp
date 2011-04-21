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

#include "ShaderCompiler.h"

// STL Includes
#include <sstream>
#include <map>
#include <fstream>

// Boost Includes
#include <boost/regex.hpp>

// Amaterasu3D Includes
#include <Utilities/StringUtils.h>
#include <Logger/Logger.h>
#include <System/MediaManager.h>

////////////////////////////////////////
/// Exception class For ShaderCompiler
////////////////////////////////////////
CShaderCompilerException::CShaderCompilerException(const std::string& message, int line)
{
	std::stringstream ss;
	ss << "[ShaderCompiler] ";
	if(line != -1)
	{
		ss << " (Line : " << line << ") ";
	}
	ss << message;
	m_Message += ss.str();
}


////////////////////////////////////////
/// Shader Compiler class
////////////////////////////////////////

ShaderCompiler::ShaderCompiler(const std::string& code, const ShaderCompilerConfig& config) :
		m_Config(config)
{
	std::vector<std::string> vectorResult;
	Split(code, vectorResult,"\n");
	m_LinesCode.insert(m_LinesCode.begin(),vectorResult.begin(),vectorResult.end());
}

ShaderCompiler::~ShaderCompiler()
{
}

void ShaderCompiler::Compile()
{
	Logger::Log() << "[INFO] Compile Current code ... \n";
	ResolveDefinesRules();
	ResolveIncludeRules();
}

void ShaderCompiler::ResolveDefinesRules()
{
	Logger::Log() << "   * ResolveDefinesRules\n";
	/////////////////////////////////////
	// Find good position to add defines.
	// After #version and #extension
	/////////////////////////////////////
	boost::regex re("[ ]*#[ ]*(version|extension)");
	std::list<std::string>::iterator it;
	for(it = m_LinesCode.begin(); it != m_LinesCode.end(); ++it)
	{
		boost::cmatch matches;
		boost::match_results<std::string::const_iterator> what;
		if(!boost::regex_search(it->c_str(),matches,re))
		{
			break;
		}
	}

	if(it == m_LinesCode.end())
		throw CShaderCompilerException("Empty source file ???");

	////////////////////////////
	// Add all defines
	////////////////////////////
	ShaderCompilerConfig::DefineMap defs = m_Config.GetDefines();
	for(ShaderCompilerConfig::DefineMap::iterator it2 = defs.begin(); it2 != defs.end(); it2++)
	{
		Logger::Log() << "   *   |- " << it2->first << " : " << it2->second << "\n";
		std::stringstream ss;
		ss << "#define " << it2->first << " " << it2->second;
		m_LinesCode.insert(it, ss.str());
	}
}

void ShaderCompiler::ResolveIncludeRules()
{
	bool find = false;
	Logger::Log() << "   * ResolveIncludeRules\n";
	/*
	 * Search Pass
	 */
	//XXX: Add multiple include support
	boost::regex re("[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
	std::map<std::string,std::list<std::string>::iterator> IncludeV;
	for(std::list<std::string>::iterator it = m_LinesCode.begin(); it != m_LinesCode.end(); ++it)
	{
		boost::cmatch matches;
		boost::match_results<std::string::const_iterator> what;
		if(boost::regex_search(it->c_str(),matches,re))
		{
			find= true;
			std::string fileName(matches[1].first, matches[1].second);
			Logger::Log() << "[Compiler] Found include : " << fileName << "\n";
			IncludeV[fileName] = it;
		}
	}
	/*
	 * Modify code pass
	 */
	for(std::map<std::string,std::list<std::string>::iterator>::iterator it = IncludeV.begin(); it != IncludeV.end(); ++it)
	{
		// Delete the current line
		std::list<std::string>::iterator pos = m_LinesCode.erase(it->second);
		// Add text code
		// * Find file
		std::string path = CMediaManager::Instance().FindMedia(it->first).Fullname();
		// * Open file
		std::string includeCode = LoadFile(path);
		std::vector<std::string> vectorResult;
		Split(includeCode, vectorResult,"\n");
		m_LinesCode.insert(pos,vectorResult.begin(),vectorResult.end());
	}

	if(find)
	{
		Logger::Log() << "Code : \n" << GetCode() << "\n";
	}
}

void ShaderCompiler::AnalyseCompilerLog(const std::string& log)
{
	std::vector<std::string> vectorResult;
	Split(log, vectorResult,"\n");
	boost::regex re("(ERROR:)\\s[0-9]:([0-9]*):\\s([\\s\\D\\W\\d]*)");
	for(std::vector<std::string>::iterator it = vectorResult.begin(); it != vectorResult.end(); ++it)
	{
		Logger::Log() << "[LOG] " << (*it) << "\n";
		boost::cmatch matches;
		boost::match_results<std::string::iterator> what;
		if(boost::regex_search(it->c_str(),matches,re))
		{
			std::string lineNumberString(matches[2].first, matches[2].second);
			std::stringstream ss;
			ss << lineNumberString;
			int lineNumber;
			ss >> lineNumber;
			Logger::Log() << "[Compiler] Error line found : " << lineNumber << "\n";
			std::list<std::string>::iterator i = m_LinesCode.begin();
			std::advance(i, lineNumber-1);
			Logger::Log() << "[Compiler] [Line] " << lineNumber << " : " << *i << "\n";
		}
	}
}

const std::string ShaderCompiler::LoadFile(const std::string& path)
{
	std::ifstream file(path.c_str(), std::ios::in);
	// Check if the file is open
	if(!file) // if not
	{
		throw CLoadingFailed(path, "enable to load the source shader");
	}
	std::stringstream buffer;
	// read all the file
	buffer << file.rdbuf();
	//Close the file
	file.close();
	// retrun the buffer
	return buffer.str();
}

const std::string ShaderCompiler::GetCode() const
{
	std::stringstream res;
	for(std::list<std::string>::const_iterator it = m_LinesCode.begin(); it != m_LinesCode.end(); ++it)
	{
		res  << (*it) << "\n";
	}
	return res.str();
}
