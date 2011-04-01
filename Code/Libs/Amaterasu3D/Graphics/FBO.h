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
#include <Graphics/Shaders/Shader.h>
#include <Graphics/Texture.h>
#include <Enum.h>
#include <map>
#include <string>

// To solve mutual inclusion
class Shader;
typedef CSmartPtr<Shader, CResourceCOM> TShaderPtr;

///////////////////////////////////////////
/// Differents type of Buffer
///////////////////////////////////////////

/**
 * \class FBOBufferParam
 * \brief based class for all FBO buffers parameters
 */
class FBOBufferParam : public Texture2DParams
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

/**
 * \class FBOTextureBufferParam
 * \brief Describe texture parameters for the Depth buffer
 */
class FBOTextureBufferParam : public FBOBufferParam
{
public:
	int Attachment;
	FBOTextureBufferParam(int attachment = 0) :
		Attachment(attachment)
	{
	}
};

/**
 * \class FBODepthBufferParam
 * \brief Describe texture attributs for Colored buffers
 */
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
	}
};

/**
 * \class FBO
 * \brief describe an FBO object. With buffers and others stuffs.
 */
class FBO
{
private:
	/*
	 * Attributes
	 */
	//! Color buffers
	std::map<std::string, Texture*> m_ColoredBuffers;
	// Attributs for the depth buffer
	FBODepthType m_DepthType; ///< the type of the depth buffer
	GLuint m_DepthID; ///< The id of the depth buffer
	// Other attributes
	GLuint m_FBO_ID; ///< OpenGL FBO id
	Math::TVector2I m_SizeBuffers; ///< The size of associated buffers
	bool m_IsActivated; ///< To know if the FBO is activated
	TShaderPtr m_DepthShader; ///< To display the Depth buffer in more good conditions
	// Attributes for copy
	// XXX: Revoir comment faire pour eviter ses attributs
	// peut etre en ajoutant une methode CShaderManager ??? LoadFileMultiple ?
	std::map<std::string, FBOTextureBufferParam> m_BuffersParams;
	FBODepthBufferParam m_DepthParams;
public:
	/*
	 * Constructors & Destructors
	 */
	FBO(const Math::TVector2I& size,
	    std::map<std::string, FBOTextureBufferParam>& buffers,
	    FBODepthType type,
	    FBODepthBufferParam& paramDepth);
	virtual ~FBO();

	/*
	 * Public methods
	 */
	//! To enable the FBO
	void Bind();
	//! To disable the FBO
	void UnBind();
	//! Draw all associates buffer in a single frame (EXPERIMENTAL)
	void DrawDebug();
	//! To get Color buffers form the FBO. It possible to get in this way the Depth buffer if is type is FBODEPTH_TEXTURE (ID: "Depth").
	Texture* GetTexture(const std::string& nameBuffer);
	//! Other technique to get the depth buffer
	GLuint GetDepthID();
	//! Return an copy of this FBO
	FBO* Copy();
private:
	/*
	 * Private methods
	 */
	int GetMaxColorAttachement();
};

#endif /* FBO_H_ */
