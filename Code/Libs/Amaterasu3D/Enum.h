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
#pragma once
#include <GL/glew.h>
#include <GL/gl.h>

enum ShaderUnitType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

enum MatrixType
{
	VIEW_MATRIX,
	MODEL_MATRIX,
	PROJECTION_MATRIX,
	NORMAL_MATRIX
};

//FIXME: Just compute the direction for Tangante and store in vec4 (last comp)
enum ShaderAttributType
{
	VERTEX_ATTRIBUT,
	TEXCOORD_ATTRIBUT,
	NORMAL_ATTRIBUT,
	TANGENT_ATTRIBUT,
	BITANGENT_ATTRIBUT,
	COLOR_ATTRIBUT
};

enum MaterialType
{
	DIFFUSE_MATERIAL,
	SPECULAR_MATERIAL,
	AMBIANT_MATERIAL,
	EMISSION_MATERIAL
};

enum TextureType
{
	DIFFUSE_TEXTURE = 0,
	NORMAL_TEXTURE = 1,
	SPECULAR_TEXTURE = 2,
	CUSTOM_TEXTURE = 3 // Need to be on the end
};

enum ShaderType
{
	BASIC_SHADER,
	GBUFFER_SHADER
};

enum FBODepthType
{
	FBODEPTH_NONE,
	FBODEPTH_TEXTURE,
	FBODEPTH_RENDERTARGET
};
