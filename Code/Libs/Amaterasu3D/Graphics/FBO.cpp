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

#include "FBO.h"
#include <Debug/Exceptions.h>
#include <Logger/Logger.h>

FBO::FBO(const Math::TVector2I& size,
		std::map<std::string, FBOTextureBufferParam>& buffers,
		FBODepthType type,
		FBODepthBufferParam& paramDepth) :
		m_depth_type(type),
		m_depth_id(0),
		m_is_activated(false)//,
		//m_size(size)
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
			m_shader_depth = CShaderManager::Instance().LoadShader("2DFBODrawDepth.shader");
			glGenTextures(1,&m_depth_id);
			glBindTexture(GL_TEXTURE_2D, m_depth_id);
			paramDepth.applyParam();
			glTexImage2D( GL_TEXTURE_2D, 0, paramDepth.InternalFormat, size.y, size.x, 0, paramDepth.ExternalFormat, paramDepth.Precision, 0);
			Logger::Log() <<" * Generate Depth Texture : " << m_depth_id << "\n";
			m_textures["Depth"] = new Texture(size, false, m_depth_id);
		}
		else if(type == FBODEPTH_RENDERTARGET)
		{
			glGenRenderbuffers(1, &m_depth_id);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depth_id);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.y, size.x);
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
	int sizeBufferDraw = buffers.size();

	GLenum buffersDraw[sizeBufferDraw];
	int i = 0;
	Logger::Log() << " * Attach textures ... \n";
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{
		Logger::Log() << "   * Texture : " << it->first << " attachment : " << it->second.Attachment << " ( id " << m_textures[it->first]->getIdTex() << " ) \n";
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+it->second.Attachment, GL_TEXTURE_2D, m_textures[it->first]->getIdTex(), 0);
		buffersDraw[i] = (GL_COLOR_ATTACHMENT0+it->second.Attachment);
		i++;
	}

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

	if(sizeBufferDraw > 0)
		glDrawBuffers(sizeBufferDraw, buffersDraw);
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(error != GL_FRAMEBUFFER_COMPLETE)
	{
		switch(error)
				{
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
						Logger::Log() << "Error! missing a required image/buffer attachment!\n";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
						Logger::Log() << "Error! has no images/buffers attached!\n";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
						Logger::Log() << "Error! has mismatched image/buffer dimensions!\n";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
						Logger::Log() << "Error! 's colorbuffer attachments have different types!\n";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
						Logger::Log() << "Error! trying to draw to non-attached color buffer!\n";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
						Logger::Log() << "Error! trying to read from a non-attached color buffer!\n";
						break;
					case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
						Logger::Log() << "Error! format is not supported by current graphics card/driver!\n";
						break;
					default:
						Logger::Log() << "*UNKNOWN ERROR* reported from glCheckFramebufferStatusEXT()!\n";
						break;
				}

		throw CException("error on FBO creation...");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

//	if(sizeBufferDraw == 0)
//	{
//		//FIXME: Only for double buffer (GL_FRONT otherwise)
//		glDrawBuffer(GL_BACK);
//		glReadBuffer(GL_BACK);
//	}
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

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //GL_VIEWPORT_BIT
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
//	glViewport(0,0,m_size.y, m_size.x);

	Logger::Log() << "BIND FBO\n";

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

	Logger::Log() << "UNBIND FBO\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPopAttrib();

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
	int nbElementsToDraw = m_textures.size();
	// * Compute the grid dimension
	int nbHeight = nbElementsToDraw / 2;
	if(nbElementsToDraw % 2 != 0)
		nbHeight++;

	int nbWidth = nbHeight;
	if(nbHeight*nbHeight < nbElementsToDraw)
		nbWidth++;

	glDisable(GL_DEPTH_TEST);

	glPushAttrib(GL_VIEWPORT_BIT);

	// Init right matrix for the drawing
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Draw the Grid
	const int WindowHeight = 600;
	const int WindowWidth = 800;
	// Compute factors width
	const int factorWidth = (WindowWidth / nbWidth);
	const int factorHeight = (WindowHeight / nbHeight);
	int nbElementDrew = 0;
	// Draw others textures
	for(std::map<std::string, Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		int idWidth = nbElementDrew / nbWidth;
		int idHeight = nbElementDrew % nbHeight;
//		Logger::Log() << it->first << " : " << idHeight*factorHeight << "x" << idWidth*factorWidth << " ( " <<factorHeight << "x" << factorWidth << ") \n";

		if(it->first == "Depth")
			m_shader_depth->begin();

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

		if(it->first == "Depth")
			m_shader_depth->end();

		it->second->desactivateTextureMapping();
		nbElementDrew++;
	}

	glEnable(GL_DEPTH_TEST);

	// Undo transform
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();

}

GLuint FBO::GetDepthID()
{
	Assert(m_depth_type == FBODEPTH_TEXTURE);
	return m_depth_id;
}
