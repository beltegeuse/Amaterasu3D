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
#include <iostream>
#include <stdlib.h>

#include <Logger/Logger.h>
#include <Debug/Exceptions.h>
#include <Graphics/MatrixManagement.h>
#include <Graphics/Shaders/GBufferShader.h>
#include <System/ResourceManager.h>
#include <System/MediaManager.h>

SINGLETON_IMPL(CShaderManager)

///////////////////
// Definition de l'exception
///////////////////
CShaderException::CShaderException(const std::string& message) :
			CException("[ShaderException] : " + message)
{
}

#define CheckLoc(Func) \
	GLint loc = GetUniformLocation(varname); \
	if(loc != -1) \
		Func;

Shader::Shader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader, ShaderUnit * GeometryShader) :
m_VertexShader(VertexShader),
m_FragementShader(FragmentShader),
m_GeometryShader(GeometryShader),
m_FBO(0),
m_IsLink(false)
{
	// Program creation
	m_ProgramID = glCreateProgram () ;
	std::cout << "[INFO] Shader creation : " << m_ProgramID << std::endl;
	// attach shaders
	Link();
}
//Destructor
Shader::~Shader()
{
	std::cout << "[DEBUG] Delete shader : " << m_ProgramID << " ..." << std::endl;
	// Free OpenGL
	glDetachShader(m_ProgramID, m_VertexShader->GetID());
	glDetachShader(m_ProgramID, m_FragementShader->GetID());
	glDeleteShader(m_ProgramID);
	// Delete shaders
	delete m_VertexShader;
	delete m_FragementShader;
}

void Shader::Link()
{
	Logger::Log() << "[INFO] Link the shader program ... \n";
	if(m_IsLink)
	{
		Logger::Log() << "[Warning] the shader program is already link ... \n";
		glDetachShader (m_ProgramID, m_VertexShader->GetID()) ;
		glDetachShader (m_ProgramID, m_FragementShader->GetID()) ;
		if(m_GeometryShader)
			glDetachShader (m_ProgramID, m_GeometryShader->GetID()) ;
	}
	glAttachShader (m_ProgramID, m_VertexShader->GetID()) ;
	glAttachShader (m_ProgramID, m_FragementShader->GetID()) ;
	if(m_GeometryShader)
		glAttachShader (m_ProgramID, m_GeometryShader->GetID()) ;
	// link the program
	glLinkProgram (m_ProgramID) ;
	ShowLinkLog(m_ProgramID);
	m_IsLink = true;
}

void Shader::Begin()
{
	glUseProgram(m_ProgramID);
	CShaderManager::Instance().Push(this);
	if(m_FBO)
			m_FBO->Bind();
}

void Shader::End()
{
	if(m_FBO)
		m_FBO->UnBind();

	glUseProgram(0);
	CShaderManager::Instance().Pop();
}

GLuint Shader::GetProgramObject()
{
	return m_ProgramID;
}

void Shader::SetGeometryShaderParameters(GLenum inputMode, GLenum outputMode, int output)
{
	Assert(m_GeometryShader != 0);
	glProgramParameteri(m_ProgramID,GL_GEOMETRY_INPUT_TYPE,inputMode);
	glProgramParameteri(m_ProgramID,GL_GEOMETRY_OUTPUT_TYPE,outputMode);

	int MaxVertices = MaxOutputVertices();
	if(output < 0)
	{
		glProgramParameteri(m_ProgramID,GL_GEOMETRY_VERTICES_OUT,MaxVertices);
	}
	else
	{
		if(output > MaxVertices)
			throw CException("Reach max output vertices");
		glProgramParameteri(m_ProgramID,GL_GEOMETRY_VERTICES_OUT,output);
	}
}

int Shader::MaxOutputVertices()
{
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES,&temp);
	return temp;
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

void Shader::AddAttributBinding(ShaderAttributType type, const std::string& name)
{
	m_attributs_bind[type] = name;
}

void Shader::UpdateAttributBinding()
{
	for(MapAttributs::iterator it = m_attributs_bind.begin(); it != m_attributs_bind.end(); it++)
	{
		BindAttribLocation(it->first, it->second.c_str());
	}
}

bool Shader::IsAttributAvailable(ShaderAttributType type)
{
	return m_attributs_bind.find(type) != m_attributs_bind.end();
}

void Shader::AddTextureUnit(int typeID, const std::string& name)
{
	m_textures_bind[typeID] = name;
}

void Shader::UpdateTextureUnitsBinding()
{
	for(MapTexture::iterator it = m_textures_bind.begin(); it != m_textures_bind.end(); it++)
	{
		Logger::Log() << "[INFO] Texture : " << it->second << " : " << (GLint)it->first << "\n";
		SetUniform1i(it->second.c_str(), it->first);
	}
}

bool Shader::IsTextureAvailable(TextureType type)
{
	return m_textures_bind.find(type) != m_textures_bind.end();
}

void Shader::AddMatrixBinding(MatrixType type, const std::string& name)
{
	m_matrix_bind[type] = name;
}

bool Shader::IsMatrixModeAvailable(MatrixType type)
{
	return m_matrix_bind.find(type) != m_matrix_bind.end();
}

void Shader::UpdateMatrix(MatrixType type)
{
	if(IsMatrixModeAvailable(type))
	{
		SetUniformMatrix4fv(type, CMatrixManager::Instance().GetMatrix(type));
	}
}

void Shader::UpdateMatrixAll()
{
	for(MapMatrix::iterator it = m_matrix_bind.begin(); it != m_matrix_bind.end();++it)
	{
		UpdateMatrix(it->first);
	}
}

void Shader::AddMaterialBinding(MaterialType type, const std::string& name)
{
	m_material_bind[type] = name;
}

bool Shader::IsMaterialAvailable(MaterialType type)
{
	return m_material_bind.find(type) != m_material_bind.end();
}

void Shader::SetMaterialValue(MaterialType type, Color& color)
{
	SetUniformColor(m_material_bind[type].c_str(), color);
}

void Shader::OnDraw()
{
  // Nothing to do ....
}

void Shader::UpdateAll()
{
	UpdateAttributBinding();
	Link();
	// * Warning : Need to active shader
	Begin();
	UpdateTextureUnitsBinding();
	End();
}

FBO* Shader::GetFBO()
{
	return m_FBO;
}

void Shader::SetFBO(FBO* fbo, bool wantDelete)
{
	if(m_FBO && wantDelete)
		delete m_FBO;
	m_FBO = fbo;
}

bool Shader::IsFBOAvaliable() const
{
	return m_FBO == NULL;
}

void  Shader::BindAttribLocation(GLint index, const GLchar* name)
{
	glBindAttribLocation(m_ProgramID, index, name);
}

/////////////////////////////////////////////////
/// Uniform Setters
/////////////////////////////////////////////////
/*
 * 1 Dimension Setters
 */
void Shader::SetUniform1f(const GLcharARB* varname, GLfloat v0)
{
	CheckLoc(glUniform1f(loc, v0))
}
void Shader::SetUniform1i(const GLcharARB* varname, GLint v0)
{
	CheckLoc(glUniform1i(loc, v0))

}
void Shader::SetUniform1ui(const GLcharARB* varname, GLuint v0)
{
	CheckLoc(glUniform1ui(loc, v0))
}

/*
 * 2 Dimension Setters
 */
void Shader::SetUniformVector(const GLcharARB* varname, const Math::TVector2F& vector)
{
	CheckLoc(glUniform2f(loc, vector.x,vector.y))
}
/*
 * 3 Dimension Setters
 */
void Shader::SetUniformVector(const GLcharARB* varname, const Math::TVector3F& vector)
{
	CheckLoc(glUniform3f(loc, vector.x,vector.y,vector.z))
}

/*
 *  4 Dimension Settings
 */
void Shader::SetUniformColor(const GLcharARB* varname, Color& color)
{
	CheckLoc(glUniform4f(loc, color.R,color.G,color.B,color.A))
}

void Shader::SetUniformVector(const GLcharARB* varname, const Math::TVector4F& vector)
{
	CheckLoc(glUniform4f(loc, vector.x, vector.y, vector.z, vector.w))
}

/*
 * 4x4 Matrix setter
 */
void Shader::SetUniformMatrix4fv(const GLchar* varname, const Math::CMatrix4& matrix)
{
	CheckLoc(glUniformMatrix4fv(loc,1, GL_FALSE, (const float*)matrix))
//	Logger::Log() << "[DEBUG] " << varname << " : "  << loc << "\n";
//	Logger::Log() << matrix << "\n";
}

void Shader::SetUniformMatrix4fv(MatrixType type, const Math::CMatrix4& matrix)
{
	Assert(IsMatrixModeAvailable(type));
	SetUniformMatrix4fv(m_matrix_bind[type].c_str(), matrix);
}

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
////////////////// ShaderManager  ////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
CShaderManager::CShaderManager() :
	m_max_stack(100)
{
	//InitOpenGLExtensions();
	CMatrixManager::Instance().GetSignalEvent().connect(sigc::mem_fun(*this, &CShaderManager::UpdateMatrix));
}

CShaderManager::~CShaderManager()
{
	// free objects
	std::vector<Shader*>::iterator  i=_shaderObjectList.begin();
	while (i!=_shaderObjectList.end())
	{
		//glShader* o = *i;
		i=_shaderObjectList.erase(i);
		//delete o;
	}
}

bool  CShaderManager::free(Shader* o)
{
	std::vector<Shader*>::iterator  i=_shaderObjectList.begin();
	while (i!=_shaderObjectList.end())
	{
		if ((*i)==o)
		{
			_shaderObjectList.erase(i);
			delete o;
			return true;
		}
		i++;
	}
	return false;
}

void CShaderManager::Push(Shader* shader)
{
	if((int)m_shader_stack.size() > m_max_stack)
	{
		std::stringstream ss;
		ss << m_shader_stack.size() << "(" << m_max_stack << ")";
		throw CException("shader stack is full "+ss.str());
	}
	m_shader_stack.push_back(shader);
	m_shader_stack.back()->UpdateMatrixAll();
}

void CShaderManager::Pop()
{
	if(m_shader_stack.empty())
		throw CException("shader stack is empty");
	m_shader_stack.pop_back();

	if(!m_shader_stack.empty())
	{
		m_shader_stack.back()->Begin();
		m_shader_stack.back()->UpdateMatrixAll();
	}
}

void CShaderManager::UpdateMatrix(MatrixType type)
{
	if(!m_shader_stack.empty())
	{
		currentShader()->UpdateMatrix(type);
	}
}

TShaderPtr CShaderManager::LoadShader(const std::string& filename)
{
	TShaderPtr Resource = CResourceManager::Instance().Get<Shader>(filename);
	if(Resource == NULL)
	{
		Resource = CMediaManager::Instance().LoadMediaFromFile<Shader>(filename);
		CResourceManager::Instance().Add(filename, Resource);
	}
	return Resource;
}

Shader* CShaderManager::CreateShader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader, ShaderUnit * GeometryShader, ShaderType type)
{
	if(type == BASIC_SHADER)
	{
		return new Shader(VertexShader,FragmentShader,GeometryShader);
	}
	else if(type == GBUFFER_SHADER)
	{
		return new GBufferShader(VertexShader,FragmentShader,GeometryShader);
	}
	else
	{
		throw CException("Unknown shader type");
	}
}

Shader* CShaderManager::loadfromFile(const char* vertexFile, const char* fragmentFile, ShaderType type)
{
	return CreateShader(new ShaderUnit(vertexFile, VERTEX_SHADER),new ShaderUnit(fragmentFile, FRAGMENT_SHADER), 0, type);
}

Shader* CShaderManager::loadfromFile(const char* vertexFile, const char* fragmentFile, const char* geometryFile, ShaderType type)
{
	return CreateShader(new ShaderUnit(vertexFile, VERTEX_SHADER),new ShaderUnit(fragmentFile, FRAGMENT_SHADER), new ShaderUnit(geometryFile, GEOMETRY_SHADER), type);
}

Shader* CShaderManager::currentShader()
{
	if(!activedShader())
		throw CException("No activated shader...");
	return m_shader_stack.back();
}

bool CShaderManager::activedShader()
{
	return m_shader_stack.size() != 0;
}
