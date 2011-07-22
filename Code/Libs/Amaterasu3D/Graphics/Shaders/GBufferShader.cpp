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

#include "GBufferShader.h"
namespace ama3D
{
GBufferShader::GBufferShader(ShaderUnit * VertexShader,
		ShaderUnit * FragmentShader, ShaderUnit * GeometryShader) :
		Shader(VertexShader, FragmentShader, GeometryShader), m_use_texCoord(
				false), m_use_texNormal(false), m_use_texDiffuse(false), m_use_texSpecular(
				false), m_use_tangants(false)
{
}

GBufferShader::~GBufferShader()
{
}

// The aim of this function is to setup correctly the shader before rendering
void GBufferShader::OnDraw()
{
	Shader::OnDraw();
	// For diffuse textures
	if (m_use_texDiffuse && m_use_texCoord)
	{
		SetUniform1i("UseDiffuseTex", 1);
	}
	else
	{
		SetUniform1i("UseDiffuseTex", 0);
	}
	// For normal texture
	if (m_use_texNormal && m_use_tangants && m_use_texCoord)
	{
		SetUniform1i("UseBumpMapping", 1);
	}
	else
	{
		SetUniform1i("UseBumpMapping", 0);
	}
	// For specular texture
	if (m_use_texSpecular && m_use_texCoord)
	{
		SetUniform1i("UseSpecularTex", 1);
	}
	else
	{
		SetUniform1i("UseSpecularTex", 0);
	}
	// After that reinit all attributs for the next drawing
	m_use_texCoord = false;
	m_use_texNormal = false;
	m_use_texDiffuse = false;
	m_use_texSpecular = false;
	m_use_tangants = false;
}

bool GBufferShader::IsAttributAvailable(int typeID)
{
	//TODO: Rewrite this function
	bool res = Shader::IsAttributAvailable(typeID);
	if (!res)
		return res;
	// Update attributs
	if (typeID == TEXCOORD_ATTRIBUT)
		m_use_texCoord = true;
	else if (typeID == TANGENT_ATTRIBUT)
		m_use_tangants = true;
	// Return the good value
	return res;
}

bool GBufferShader::IsTextureAvailable(TextureType type)
{
	//TODO: Rewrite this function
	bool res = Shader::IsTextureAvailable(type);
	if (!res)
		return res;
	// Update attributs
	if (type == DIFFUSE_TEXTURE)
	{
		m_use_texDiffuse = true;
	}
	else if (type == NORMAL_TEXTURE)
	{
		m_use_texNormal = true;
	}
	else if (type == SPECULAR_TEXTURE)
		m_use_texSpecular = true;
	// Return the good value
	return res;
}
}
