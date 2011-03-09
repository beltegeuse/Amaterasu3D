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
	MODELVIEW_MATRIX,
	PROJECTION_MATRIX,
	NORMAL_MATRIX
};

enum ShaderAttributType
{
	VERTEX_ATTRIBUT,
	TEXCOORD_ATTRIBUT,
	NORMAL_ATTRIBUT,
	TANGENT_ATTRIBUT,
	COLOR_ATTRIBUT
};

enum TextureType
{
	DIFFUSE_TEXTURE = 0,
	NORMAL_TEXTURE
};
