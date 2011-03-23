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
	std::cout << "[INFO] Shader creation : " << m_programID << std::endl;
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

void Shader::SetUniformMatrix4fv(const GLchar* name, Math::CMatrix4& matrix)
{
	GLCheck(glUniformMatrix4fv(GetUniformLocation(name),1, GL_TRUE, (float*)matrix));
}

void Shader::ShowLinkLog(unsigned int id)
{
	if(id == 0)
	{
		throw CException("invalid Shader program");
	}
	GLint linked;
	glGetProgramiv(id, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		throw CException("Shader program is not link.");
	}
	GLint blen = 0;
	GLsizei slen = 0;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH , &blen);
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

