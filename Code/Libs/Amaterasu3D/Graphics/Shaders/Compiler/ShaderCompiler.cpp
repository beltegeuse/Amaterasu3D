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

// Amaterasu3D Includes
#include <Utilities/StringUtils.h>
#include <Logger/Logger.h>

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

ShaderCompiler::ShaderCompiler(const std::string& code)
{
	Split(code, m_LinesCode,"\n");
}

ShaderCompiler::~ShaderCompiler()
{
}

void ShaderCompiler::Compile()
{
	Logger::Log() << "[INFO] Compile Current code ... \n";
}

const std::string ShaderCompiler::GetCode() const
{
	std::stringstream res;
	for(std::vector<std::string>::const_iterator it = m_LinesCode.begin(); it != m_LinesCode.end(); ++it)
	{
		res  << (*it) << "\n";
	}
	return res.str();
}
