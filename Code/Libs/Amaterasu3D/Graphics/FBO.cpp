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
		m_depth_id(0),
		m_is_activated(false)
{
	Logger::Log() << "[INFO] FBO Creation ... \n";
	// On verifie que l'on a assez de Color Attachement
	if(GetMaxColorAttachement() < buffers.size())
		throw CException("Pas assez de Color attachement");

	// ==== Construction du FBO pour les color
	Logger::Log() << "  * Color buffer creation ... \n";
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{

		Texture* tex = new Texture(size);
		m_textures[it->first] = tex;
		glBindTexture(GL_TEXTURE_2D, tex->getIdTex());
		it->second.applyParam();
		glTexImage2D(GL_TEXTURE_2D, 0, it->second.InternalFormat, size.y, size.x, 0, it->second.ExternalFormat, it->second.Precision, 0);
		Logger::Log() << "   * Create new texture : " << it->first <<  " ( " << tex->getIdTex() << " )\n";
	}
	// ==== Construction de FBO pour Depth
	if(type != FBODEPTH_NONE)
	{
		Logger::Log() << "   * Depth buffer creation ... \n";
		if(type == FBODEPTH_TEXTURE)
		{
			glGenTextures(1,&m_depth_id);
			glBindTexture(GL_TEXTURE_2D, m_depth_id);
			paramDepth.applyParam();
			glTexImage2D( GL_TEXTURE_2D, 0, paramDepth.InternalFormat, size.y, size.x, 0, paramDepth.ExternalFormat, paramDepth.Precision, 0);
			Logger::Log() <<" * Generate Depth Texture : " << m_depth_id << "\n";
		}
		else if(type == FBODEPTH_RENDERTARGET)
		{
			glGenRenderbuffers(1, &m_depth_id);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depth_id);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_ARB, size.y, size.x);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
	Logger::Log() << " * Attach textures ... \n";
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{
		Logger::Log() << "   * Texture : " << it->first << " attachment : " << it->second.Attachment << " ( id " << m_textures[it->first]->getIdTex() << " ) \n";
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
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw CException("error on FBO creation...");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

FBO::~FBO()
{
	//FIXME: Faire le destructeur
	Logger::Log() << "[INFO] FBO destory \n";
}

void FBO::Bind()
{
	if(m_is_activated)
		return;

	//	Logger::Log() << "FBO Bind " << m_fbo_id << "\n";
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	//	glViewport(0,0,m_size.y, m_size.x);

	GLbitfield flags = 0;
	if(m_depth_type != FBODEPTH_NONE)
		flags = flags | GL_DEPTH_BUFFER_BIT;

	if(!m_textures.empty())
		flags = flags | GL_COLOR_BUFFER_BIT;
	else
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	if(flags)
		glClear(flags);

	m_is_activated = true;
}

void FBO::UnBind()
{
	if(!m_is_activated)
		return;

	//	Logger::Log() << "FBO UnBind " << m_fbo_id << "\n";

	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(m_textures.empty())
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	m_is_activated = false;
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

void FBO::DrawDebug()
{
	// Compute Grid need
	// * Compute how many slot need
	int nbElementsToDraw = 0;
	if(m_depth_type == FBODEPTH_TEXTURE)
		nbElementsToDraw++;
	nbElementsToDraw += m_textures.size();
//	Logger::Log() << "ElemeentDraw : " << nbElementsToDraw << "\n";
	// * Compute the grid dimension
	int nbHeight = nbElementsToDraw / 2;
	if(nbElementsToDraw % 2 != 0)
		nbHeight++;

	int nbWidth = nbHeight;
	if(nbHeight*nbHeight < nbElementsToDraw)
		nbWidth++;

	glPushAttrib(GL_VIEWPORT_BIT);

	// Init right matrix for the drawing
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Draw the Grid
	const int WindowHeight = 600; // FIXME: Add to an manager
	const int WindowWidth = 800;
	// Compute factors width
	const int factorWidth = (WindowWidth / nbWidth);
	const int factorHeight = (WindowHeight / nbHeight);
//	Logger::Log() << "Nb : " << nbHeight << "x" << nbWidth << "\n";
//	Logger::Log() << "Factors : " << factorHeight << "x" << factorWidth << "\n";
	glEnable (GL_SCISSOR_TEST);
	int nbElementDrew = 0;
	//glViewport(0,0, factorWidth,  factorHeight);
	if(m_depth_type == FBODEPTH_TEXTURE)
	{
		// Setup scissor
		//glScissor(0,0, factorWidth,  factorHeight);
		glViewport(0,0, factorWidth,  factorHeight);
		// Active the texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_depth_id);
		// Draw the texture
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
		glEnd();
		// Desactive textures
		glBindTexture(GL_TEXTURE_2D, m_depth_id);
		glDisable(GL_TEXTURE_2D);
		nbElementDrew++;
	}
	// Draw others textures
	for(std::map<std::string, Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		int idWidth = nbElementDrew / nbWidth;
		int idHeight = nbElementDrew % nbHeight;
//		Logger::Log() << it->first << " : " << idHeight*factorHeight << "x" << idWidth*factorWidth << "\n";
		// Setup scissor
		//glScissor(idWidth*factorWidth,idHeight*factorHeight, (idWidth+1)*factorWidth,(idHeight+1)*factorHeight);
		glViewport(idWidth*factorWidth,idHeight*factorHeight, factorWidth,  factorHeight);
		it->second->activateTextureMapping();
		it->second->activateTexture();
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
		glEnd();
		it->second->desactivateTextureMapping();
		nbElementDrew++;
	}

	glDisable ( GL_SCISSOR_TEST);

	// Undo transform
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

}