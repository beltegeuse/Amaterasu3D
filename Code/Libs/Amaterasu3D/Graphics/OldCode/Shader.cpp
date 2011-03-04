#include "Shader.h"
#include <fstream>
#include <sstream>
#include <Debug/Exceptions.h>
#include <iostream>
#include <stdlib.h>

// Constructeurs
Shader::Shader() :
m_is_build(false)
{
	// Empty constructor
}
Shader::Shader(const std::string& pathVertex, const std::string& pathFragment) :
m_is_build(true)
{
	m_vertexID = CompileShader(pathVertex, VERTEX) ;
	m_fragmentID = CompileShader(pathFragment, FRAGMENT) ;
	// Program creation
	m_programID = glCreateProgram () ;
	// attach shaders
	glAttachShader (m_programID, m_vertexID) ;
	glAttachShader (m_programID, m_fragmentID) ;
	// link the program
	glLinkProgram (m_programID) ;
	ShowLinkLog(m_programID);
}
//Destructor
Shader::~Shader()
{
	if(m_is_build)
	{
		std::cout << "[DEBUG] Delete shader : " << m_programID << " ..." << std::endl;
		glDetachShader(m_programID, m_vertexID);
		glDetachShader(m_programID, m_fragmentID);
		glDeleteShader(m_programID);
	}

}

void Shader::Begin()
{
	glUseProgram(m_programID);
}

void Shader::End()
{
	glUseProgram(0);
}

// Private function
/**
 * Load Shader File...
 */
const std::string Shader::LoadFile(const std::string& path)
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

void Shader::ShowLinkLog(unsigned int id)
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
		glGetProgramInfoLog(id, blen, &slen, compilerLog);
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


void Shader::ShowCompilerLog(unsigned int id)
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

unsigned int Shader::CompileShader(const std::string& path, ShaderType type)
{
	std::cout << "[INFO] Compile shader file : " << path << std::endl;
	unsigned int ID;
	// ID Creation
	if(type == VERTEX)
	{
		std::cout << "  * Shader type : VERTEX" << std::endl;
		ID = glCreateShader (GL_VERTEX_SHADER);
	}
	else if(type == FRAGMENT)
	{
		std::cout << "  * Shader type : FRAGMENT" << std::endl;
		ID = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
		throw CException("Unknow shader type ...");
	// Load the file
	const std::string source = LoadFile (path) ;
	const char * bufferPtr = source.c_str();
	glShaderSource (ID, 1, &bufferPtr, NULL) ;
	// Compile the shader
	std::cout << "  * Building ...." << std::endl;
	glCompileShader (ID) ;
	// Show all the Complilation log
	ShowCompilerLog(ID);
	// Check if it's compiled
	GLint compiled = 0;
	glGetObjectParameterivARB(ID, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
		throw CException("compile problem on "+path);
	// And just return the ID
	return ID;
}

void Shader::SetUniformMatrix4fv(const std::string& name, Math::CMatrix4& matrix)
{
	GLCheck(glUniformMatrix4fv(GetAttribLocation(name),1, GL_TRUE, (float*)matrix));
}
