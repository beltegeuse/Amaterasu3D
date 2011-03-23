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

#ifndef FBO_H_
#define FBO_H_
#include <Graphics/GLSLShader.h>
#include <Graphics/Texture.h>
#include <Enum.h>
#include <map>
#include <string>

class glShader;
typedef CSmartPtr<glShader, CResourceCOM> TShaderPtr;

class FBOBufferParam : public TextureParams
{
public:
	GLenum ExternalFormat;
	GLenum InternalFormat;
	GLenum Precision;

	FBOBufferParam() :
		ExternalFormat(GL_RGBA),
		InternalFormat(GL_RGBA),
		Precision(GL_FLOAT)
	{
		MinFiltering = GL_NEAREST;
		MaxFiltering = GL_NEAREST;
		SWrap = GL_CLAMP_TO_EDGE ;
		TWrap = GL_CLAMP_TO_EDGE ;
	}
};

class FBOTextureBufferParam : public FBOBufferParam
{
public:
	int Attachment;
	FBOTextureBufferParam(int attachment = 0) :
		Attachment(attachment)
	{
	}
};

class FBODepthBufferParam : public FBOBufferParam
{
public:
	FBODepthBufferParam()
	{
		ExternalFormat = GL_DEPTH_COMPONENT;
		InternalFormat = GL_DEPTH_COMPONENT24;
		Precision =  GL_FLOAT;
	}

	virtual void applyParam()
	{
		FBOBufferParam::applyParam();
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	}
};

class FBO
{
private:
	// Attributs
	Math::TVector2I m_size;
	std::map<std::string, Texture*> m_textures;
	FBODepthType m_depth_type;
	GLuint m_depth_id;
	GLuint m_fbo_id;
	bool m_is_activated;
	// To Avoid reloading shader
	TShaderPtr m_shader_depth;
public:
	FBO(const Math::TVector2I& size,
	    std::map<std::string, FBOTextureBufferParam>& buffers,
	    FBODepthType type,
	    FBODepthBufferParam& paramDepth);
	virtual ~FBO();

	void Bind();
	void UnBind();
	Texture* GetTexture(const std::string& nameBuffer);
	GLuint GetDepthID();

	//! Draw all associates buffer in a single frame
	void DrawDebug();
private:
	int GetMaxColorAttachement();
};

#endif /* FBO_H_ */
