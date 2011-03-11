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

enum TextureType
{
	DIFFUSE_TEXTURE = 0,
	NORMAL_TEXTURE = 1,
	SPECULAR_TEXTURE = 2
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
