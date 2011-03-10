/*
 * FBO.cpp
 *
 *  Created on: Mar 9, 2011
 *      Author: Beltegeuse
 */

#include "FBO.h"
#include <Debug/Exceptions.h>
#include <Logger/Logger.h>

FBO::FBO(const Math::TVector2I& size,
	    std::map<std::string, FBOTextureBufferParam>& buffers,
	    FBODepthType type,
	    FBODepthBufferParam& paramDepth) :
	    m_depth_type(type),
	    m_depth_id(0)
{
	// On verifie que l'on a assez de Color Attachement
	if(GetMaxColorAttachement() < buffers.size())
	   throw CException("Pas assez de Color attachement");

	// ==== Construction du FBO pour les color
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{
	   Texture* tex = new Texture(size);
	   m_textures[it->first] = tex;
	   glBindTexture(GL_TEXTURE_2D, tex->getIdTex());
	   glTexImage2D(GL_TEXTURE_2D, 0, it->second.InternalFormat, size.y, size.x, 0, it->second.ExternalFormat, it->second.Precision, 0);
	   it->second.applyParam();
	   Logger::Log() << " * Generate Image Render : " << tex->getIdTex() << "\n";
	}
	// ==== Construction de FBO pour Depth
	if(type != FBODEPTH_NONE)
	{
	   if(type == FBODEPTH_TEXTURE)
	   {
		   glGenTextures(1,&m_depth_id);
		   glBindTexture(GL_TEXTURE_2D, m_depth_id);
		   glTexImage2D( GL_TEXTURE_2D, 0, paramDepth.InternalFormat, size.y, size.x, 0, paramDepth.ExternalFormat, paramDepth.Precision, 0);
		   paramDepth.applyParam();
		   Logger::Log() <<" * Generate Depth Texture : " << m_depth_id << "\n";
	   }
	   else if(type == FBODEPTH_RENDERTARGET)
	   {
		   glGenRenderbuffers(1, &m_depth_id);
		   glBindRenderbuffer(GL_RENDERBUFFER, m_depth_id);
		   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_ARB, size.y, size.x);
		   Logger::Log() << " * Generate Depth Render target : " <<  m_depth_id << "\n";
	   }
	   else
		   throw CException("Impossible de trouver le type de Depth type.");
	}

	// ==== Creation du FBO
	glGenFramebuffers(1, &m_fbo_id);
	Logger::Log() << " * FBO ID : "  << m_fbo_id << "\n";
	glBindFramebuffer(GL_FRAMEBUFFER,m_fbo_id);

	// ==== Ajout au FBO les couleurs
	GLenum buffersDraw[buffers.size()];
	int i = 0;
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{
	   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+it->second.Attachment, GL_TEXTURE_2D, m_textures[it->first]->getIdTex(), 0);
	   buffersDraw[i] = (GL_COLOR_ATTACHMENT0+it->second.Attachment);
	   i++;
	}

	if(buffers.size() > 0)
	   glDrawBuffers(buffers.size(), buffersDraw);

	// ==== Ajout au FBO de la profondeur
	if(type != FBODEPTH_NONE)
	{
	   if(type == FBODEPTH_TEXTURE)
		   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_id, 0);
	   else if(type == FBODEPTH_RENDERTARGET)
		   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_id);
	   else
		   throw CException("Impossible de trouver le type de Depth type.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	//FIXME: A implemeter
	//checkFramebufferStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

FBO::~FBO()
{
	//FIXME: Faire le destructeur
}

void FBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0,0,m_size.y, m_size.x);

	GLbitfield flags = 0;
	if(m_depth_type != FBODEPTH_NONE)
		flags = flags | GL_DEPTH_BUFFER_BIT;

	if(!m_textures.empty())
		flags = flags | GL_COLOR_BUFFER_BIT;
	else
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	if(flags)
		glClear(flags);
}

void FBO::UnBind()
{
	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(m_textures.empty())
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

int FBO::GetMaxColorAttachement()
{
	GLint nb;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &nb);
	return nb;
}

Texture* FBO::GetTexture(const std::string& nameBuffer)
{
	Assert(m_textures.find(nameBuffer) != m_textures.end());
	return m_textures[nameBuffer];
}
