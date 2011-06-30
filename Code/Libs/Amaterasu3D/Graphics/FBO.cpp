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
		m_DepthType(type),
		m_DepthID(0),
		m_SizeBuffers(size),
		m_IsActivated(false),
		m_BuffersParams(buffers),
		m_DepthParams(paramDepth)
{
	// ============== Initialisation Debug test
	m_TextDebug.Position = Math::TVector2F(0.0,0.0);
	m_TextDebug.Size = 30;
	// ============== Creation FBO
	Logger::Log() << "[INFO] FBO Creation ... \n";
	// On verifie que l'on a assez de Color Attachement
	if(GetMaxColorAttachement() < (int)buffers.size())
		throw CException("Pas assez de Color attachement");

	// ==== Construction du FBO pour les color
	Logger::Log() << "  * Color buffer creation ... \n";
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{
		Texture* tex = new Texture();
		m_ColoredBuffers[it->first] = tex;
		glBindTexture(GL_TEXTURE_2D, tex->getIdTex());
		it->second.applyParam();
		glTexImage2D(GL_TEXTURE_2D, 0, it->second.InternalFormat, size.x, size.y, 0, it->second.ExternalFormat, it->second.Precision, 0);
		it->second.applyPostParam();
		Logger::Log() << "   * Create new texture : " << it->first <<  " ( " << tex->getIdTex() << " )\n";
	}
	// ==== Construction de FBO pour Depth
	if(type != FBODEPTH_NONE)
	{
		Logger::Log() << "   * Depth buffer creation ... \n";
		if(type == FBODEPTH_TEXTURE)
		{
			m_DepthShader = CShaderManager::Instance().LoadShader("2DFBODrawDepth.shader");
			glGenTextures(1,&m_DepthID);
			glBindTexture(GL_TEXTURE_2D, m_DepthID);
			paramDepth.applyParam();
			glTexImage2D( GL_TEXTURE_2D, 0, paramDepth.InternalFormat, size.x, size.y, 0, paramDepth.ExternalFormat, paramDepth.Precision, 0);
			paramDepth.applyPostParam();
			Logger::Log() <<" * Generate Depth Texture : " << m_DepthID << "\n";
			m_ColoredBuffers["Depth"] = new Texture(false, m_DepthID);
		}
		else if(type == FBODEPTH_RENDERTARGET)
		{
			glGenRenderbuffers(1, &m_DepthID);
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			Logger::Log() << " * Generate Depth Render target : " <<  m_DepthID << "\n";
		}
		else
			throw CException("Impossible de trouver le type de Depth type.");
	}

	// ==== Creation du FBO
	glGenFramebuffers(1, &m_FBO_ID);
	Logger::Log() << " * FBO ID : "  << m_FBO_ID << "\n";
	glBindFramebuffer(GL_FRAMEBUFFER,m_FBO_ID);

	// ==== Ajout au FBO les couleurs
	int sizeBufferDraw = buffers.size();

	GLenum* buffersDraw = new GLenum[sizeBufferDraw];
	int i = 0;
	Logger::Log() << " * Attach textures ... \n";
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = buffers.begin(); it != buffers.end(); it++)
	{
		Logger::Log() << "   * Texture : " << it->first << " attachment : " << it->second.Attachment << " ( id " << m_ColoredBuffers[it->first]->getIdTex() << " ) \n";
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+it->second.Attachment, GL_TEXTURE_2D, m_ColoredBuffers[it->first]->getIdTex(), 0);
		buffersDraw[i] = (GL_COLOR_ATTACHMENT0+it->second.Attachment);
		i++;
	}

	// ==== Ajout au FBO de la profondeur
	if(type != FBODEPTH_NONE)
	{
		if(type == FBODEPTH_TEXTURE)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthID, 0);
		else if(type == FBODEPTH_RENDERTARGET)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthID);
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

	CheckFBOStatus();

	// Delete allocated objects
	delete[] buffersDraw;
}

void FBO::CheckFBOStatus()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
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
}

void FBO::SetSize(const Math::TVector2I& size)
{
	// Update size
	m_SizeBuffers = size;

	// Update Colored buffers
	for(std::map<std::string, Texture*>::iterator it = m_ColoredBuffers.begin(); it != m_ColoredBuffers.end(); ++it)
	{
		it->second->activateTexture();
		FBOTextureBufferParam params = m_BuffersParams[it->first];
		glTexImage2D(GL_TEXTURE_2D, 0, params.InternalFormat, size.x, size.y, 0, params.ExternalFormat, params.Precision, 0);
		it->second->desactivateTextureMapping();
	}

	// Update depth buffer
	if(m_DepthType != FBODEPTH_NONE)
	{
		if(m_DepthType == FBODEPTH_TEXTURE)
		{
			m_ColoredBuffers["Depth"]->activateTexture();
			glTexImage2D( GL_TEXTURE_2D, 0, m_DepthParams.InternalFormat, size.x, size.y, 0, m_DepthParams.ExternalFormat, m_DepthParams.Precision, 0);
			m_ColoredBuffers["Depth"]->desactivateTextureMapping();
		}
		else if(m_DepthType == FBODEPTH_RENDERTARGET)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		else
			throw CException("Impossible de trouver le type de Depth type.");
	}

	CheckFBOStatus();
}

FBO::~FBO()
{
	//FIXME: Faire le destructeur
	Logger::Log() << "[INFO] FBO destory \n";
}

void FBO::Bind()
{
	if(m_IsActivated)
		return;

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
	glViewport(0,0,m_SizeBuffers.x, m_SizeBuffers.y);

	GLbitfield flags = 0;
	if(m_DepthType != FBODEPTH_NONE)
		flags = flags | GL_DEPTH_BUFFER_BIT;

	if(!m_ColoredBuffers.empty())
		flags = flags | GL_COLOR_BUFFER_BIT;
	else
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	if(flags)
		glClear(flags);

	m_IsActivated = true;
}

void FBO::UnBind()
{
	if(!m_IsActivated)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPopAttrib();

	if(m_ColoredBuffers.empty())
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// Check if Texture need to be updated
	for(std::map<std::string, FBOTextureBufferParam>::iterator it = m_BuffersParams.begin(); it != m_BuffersParams.end(); ++it)
	{
		if(it->second.NeedUpdate())
		{
			m_ColoredBuffers[it->first]->activateTextureMapping();
			m_ColoredBuffers[it->first]->activateTexture();
			it->second.applyPostParam();
			m_ColoredBuffers[it->first]->desactivateTextureMapping();
		}
	}

	m_IsActivated = false;
}

int FBO::GetMaxColorAttachement()
{
	GLint nb;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &nb);
	return nb;
}

Texture* FBO::GetTexture(const std::string& nameBuffer)
{
	Assert(m_ColoredBuffers.find(nameBuffer) != m_ColoredBuffers.end());
	return m_ColoredBuffers[nameBuffer];
}

void FBO::DrawDebug()
{
	// Compute Grid need
	// * Compute how many slot need
	int nbElementsToDraw = m_ColoredBuffers.size();
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
	for(std::map<std::string, Texture*>::iterator it = m_ColoredBuffers.begin(); it != m_ColoredBuffers.end(); ++it)
	{
		int idWidth = nbElementDrew / nbWidth;
		int idHeight = nbElementDrew % nbHeight;

		if(it->first == "Depth")
			m_DepthShader->Begin();

		glViewport(idWidth*factorWidth,idHeight*factorHeight, factorWidth,  factorHeight);

		// Draw the buffer content
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
			m_DepthShader->End();

		// Draw name buffer
		m_TextDebug.Text = it->first;
		m_TextDebug.Draw();


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
	Assert(m_DepthType == FBODEPTH_TEXTURE);
	return m_DepthID;
}

FBO* FBO::Copy()
{
	return new FBO(m_SizeBuffers,m_BuffersParams,m_DepthType,m_DepthParams);
}
