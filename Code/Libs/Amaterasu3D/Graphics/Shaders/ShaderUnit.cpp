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

#include <GL/glew.h>
#ifdef MSVC
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include "ShaderUnit.h"

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>

#include <Exceptions.h>
#include <iostream> //#include "Logger.h"
#include <Utilities/StringUtils.h>
//#include <Graphics/Shaders/Compiler/ShaderCompiler.h>
#include <System/Python/Jinja2Template.h>

//*********************************************************
//*********************************************************
//*********************************************************
//************** ShaderUnit class *************************
//*********************************************************
//*********************************************************
//*********************************************************
namespace ama3D
{

void AddArgument(Jinja2Template& jinja2, const std::string& name,const ShaderCompilerConfig::DefineEntry& entry)
{
	switch(entry.type)
	{
		case ShaderCompilerConfig::DEFINE_BOOL:
			jinja2.AddArgument(name, ToBool(entry.value));
			break;
		case ShaderCompilerConfig::DEFINE_STRING:
			jinja2.AddArgument(name, entry.value);
			break;
		case ShaderCompilerConfig::DEFINE_INTEGER:
			jinja2.AddArgument(name, (long)ToIntType()(entry.value));
			break;
		case ShaderCompilerConfig::DEFINE_FLOAT:
			jinja2.AddArgument(name, ToFloatType()(entry.value));
			break;
		default:
			throw CException("Not supported type");
	}
}

ShaderUnit::ShaderUnit(const std::string& path, const ShaderUnitType& type,
		const ShaderCompilerConfig& config)
{
	std::cout << "[INFO] Compile shader file : " << path << "\n";
	// ID Creation
	if (type == VERTEX_SHADER)
	{
		std::cout << "  * Shader type : VERTEX \n";
		m_ID = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	}
	else if (type == FRAGMENT_SHADER)
	{
		std::cout << "  * Shader type : FRAGMENT \n";
		m_ID = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	}
	else if (type == GEOMETRY_SHADER)
	{
		std::cout << "  * Shader type : GEOMETRY \n";
		m_ID = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
	}
	else
		throw CException("Unknow shader type ...");
	// Use own shader Compiler to add extra stuff to GLSL langage
	Jinja2Template jinja2(path);
	for(ShaderCompilerConfig::DefineMap::const_iterator it = config.GetDefines().begin(); it != config.GetDefines().end(); ++it)
		AddArgument(jinja2,it->first, it->second);
	jinja2.Generate();
	std::cout << jinja2.GetCode() << "\n";
	const std::string source = jinja2.GetCode();
	// Use GLSL To compile the current shader code
	const char * bufferPtr = source.c_str();
	GLint lenght = source.size();
	glShaderSource(m_ID, 1, &bufferPtr, &lenght);
	// Compile the shader
	std::cout << "  * Building .... \n";
	glCompileShader(m_ID);
	// Show all the Complilation log
	ShowCompilerLog(m_ID);
	// Check if it's compiled
	GLint compiled = 0;
	glGetObjectParameterivARB(m_ID, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
		throw CException("compile problem on " + path);
}

ShaderUnit::~ShaderUnit()
{
}

void ShaderUnit::ShowCompilerLog(unsigned int id)
{
	if (id == 0)
	{
		throw CException("invalid Shader program");
	}
	GLint blen = 0;
	GLsizei slen = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &blen);
	if (blen > 1)
	{
		GLcharARB* compilerLog = (GLcharARB*) malloc(blen);
		glGetInfoLogARB(id, blen, &slen, compilerLog);
		if (compilerLog != 0)
		{
			std::cout << "[LOG] **** Compiler LOG **** \n";
			std::cout << compilerLog;
			//compiler.AnalyseCompilerLog(std::string(compilerLog));
			std::cout << "[LOG] **** END **** \n";
		}
		else
		{
			std::cout << "[LOG] Compiler LOG : No log available \n";
		}
		free(compilerLog);
	}
}

unsigned int ShaderUnit::GetID()
{
	return m_ID;
}

// Private function
/**
 * Load Shader File...
 */
const std::string ShaderUnit::LoadFile(const std::string& path)
{
	std::cout << "[INFO] LoadFile : " << path << "\n";
	std::ifstream file(path.c_str(), std::ios::in);
	// Check if the file is open
	if (!file) // if not
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
}
