/*
 * ShaderUnit.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#include "ShaderUnit.h"
#include <iostream>
#include <Debug/Exceptions.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>

//*********************************************************
//*********************************************************
//*********************************************************
//************** ShaderUnit class *************************
//*********************************************************
//*********************************************************
//*********************************************************


ShaderUnit::ShaderUnit(const std::string& path, const ShaderUnitType& type)
{
	std::cout << "[INFO] Compile shader file : " << path << std::endl;
	// ID Creation
	if(type == VERTEX_SHADER)
	{
		std::cout << "  * Shader type : VERTEX" << std::endl;
		m_ID = glCreateShader (GL_VERTEX_SHADER);
	}
	else if(type == FRAGMENT_SHADER)
	{
		std::cout << "  * Shader type : FRAGMENT" << std::endl;
		m_ID = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
		throw CException("Unknow shader type ...");
	// Load the file
	const std::string source = LoadFile (path) ;
	const char * bufferPtr = source.c_str();
	glShaderSource (m_ID, 1, &bufferPtr, NULL) ;
	// Compile the shader
	std::cout << "  * Building ...." << std::endl;
	glCompileShader (m_ID) ;
	// Show all the Complilation log
	ShowCompilerLog(m_ID);
	// Check if it's compiled
	GLint compiled = 0;
	glGetObjectParameterivARB(m_ID, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
		throw CException("compile problem on "+path);
}

ShaderUnit::~ShaderUnit()
{
}

void ShaderUnit::ShowCompilerLog(unsigned int id)
{
	if(id == 0)
	{
		throw CException("invalid Shader program");
	}
	GLint blen = 0;
	GLsizei slen = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH , &blen);
	if (blen > 1)
	{
		GLcharARB* compilerLog = (GLcharARB*)malloc(blen);
		glGetInfoLogARB(id, blen, &slen, compilerLog);
		if(compilerLog!=0)
		{
			std::cout << "[LOG] **** Compiler LOG **** " << std::endl;
			std::cout << compilerLog;
			std::cout << "[LOG] **** END **** " << std::endl;
		}
		else
		{
			std::cout << "[LOG] Compiler LOG : No log available" << std::endl;
		}
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