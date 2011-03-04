#include "Shader.h"
#include <fstream>
#include <sstream>
#include <Debug/Exceptions.h>
#include <iostream>
#include <stdlib.h>

Shader::Shader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader) :
m_vertex_shader(VertexShader),
m_fragement_shader(FragmentShader)
{
	// Program creation
	m_programID = glCreateProgram () ;
	// attach shaders
	glAttachShader (m_programID, m_vertex_shader->GetID()) ;
	glAttachShader (m_programID, m_fragement_shader->GetID()) ;
	// link the program
	glLinkProgram (m_programID) ;
	ShowLinkLog(m_programID);
}
//Destructor
Shader::~Shader()
{
	std::cout << "[DEBUG] Delete shader : " << m_programID << " ..." << std::endl;
	// Free OpenGL
	glDetachShader(m_programID, m_vertex_shader->GetID());
	glDetachShader(m_programID, m_fragement_shader->GetID());
	glDeleteShader(m_programID);
	// Delete shaders
	delete m_vertex_shader;
	delete m_fragement_shader;


}

void Shader::Begin()
{
	glUseProgram(m_programID);
}

void Shader::End()
{
	glUseProgram(0);
}

void Shader::SetUniformMatrix4fv(const std::string& name, Math::CMatrix4& matrix)
{
	GLCheck(glUniformMatrix4fv(GetUniformLocation(name),sizeof(float), GL_TRUE, (float*)matrix));
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

