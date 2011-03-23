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
#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <Math/Matrix4.h>
#include <Singleton.h>
#include <Debug/OpenGLDebug.h>
#include <System/Resource.h>
#include <Graphics/ShaderUnit.h>
#include <iostream>
#include <Enum.h>

/*
 * \brief class which describe one shader
 */


class Shader : public IResource
{
private:
	// Attributs
	ShaderUnit * m_vertex_shader;
	ShaderUnit * m_fragement_shader;
	unsigned int m_programID;
public:
	// Constructor
	Shader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader);
	virtual ~Shader();
	// Public functions
	void Begin();
	void End();
	void SetUniformMatrix4fv(const GLchar* name, Math::CMatrix4& matrix);
private:
	void ShowLinkLog(unsigned int id);
	// Common shader methods
	inline int GetAttribLocation(const GLchar* name);
	inline int GetUniformLocation(const GLchar* name);
};

//***********************
//** Inline functions ***
//***********************
int Shader::GetAttribLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetAttribLocation (m_programID,name));
	//std::cout << "[DEBUG] AttribLocation : " << name << " => " << id << std::endl;
	return id;
}

int Shader::GetUniformLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	GLint loc = GLCheck(glGetUniformLocation(m_programID,name));
	std::cout << "[DEBUG] UniformLocation : " << name << " => " << loc << " ( program " << m_programID << " )" <<  std::endl;
	return loc;
}

#endif /* SHADER_H_ */
