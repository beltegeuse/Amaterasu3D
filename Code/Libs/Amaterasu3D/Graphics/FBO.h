/*
 * FBO.h
 *
 *  Created on: Mar 9, 2011
 *      Author: Beltegeuse
 */

#ifndef FBO_H_
#define FBO_H_

#include <Graphics/Texture.h>
#include <Enum.h>
#include <vector>

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
		SWrap = GL_CLAMP;
		TWrap = GL_CLAMP;
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
		InternalFormat = GL_DEPTH_COMPONENT;
		Precision = GL_UNSIGNED_BYTE;
	}
};

class FBO
{
private:
	// Attributs
	Math::TVector2I m_size;
	std::vector<GLuint> m_image_id;
	FBODepthType m_depth_type;
	GLuint m_depth_id;
	GLuint m_fbo_id;
public:
	FBO(const Math::TVector2I& size,
	    std::vector<FBOTextureBufferParam>& buffers,
	    FBODepthType type,
	    FBODepthBufferParam& paramDepth);
	virtual ~FBO();

	void Bind();
	void UnBind();
private:
	int GetMaxColorAttachement();
};

#endif /* FBO_H_ */
