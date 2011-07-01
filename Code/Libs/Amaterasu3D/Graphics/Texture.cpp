//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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
#include <GL/glew.h>
#include <GL/gl.h>

#include "Texture.h"

#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <System/MediaManager.h>
#include <System/ResourceManager.h>

namespace ama3D
{
//Constructeur
Texture::Texture(bool create, GLuint id, GLenum textureMode) :
		m_TextureMode(textureMode)
{
	if (create)
		glGenTextures(1, &m_idTex);
	else
		m_idTex = id;
}

//Destructeur
Texture::~Texture()
{
	glDeleteTextures(1, &m_idTex);
	Logger::Log() << "[INFO] Delete texture... \n";
}

//Fonction qui active le texturage
void Texture::activateTextureMapping()
{
	glEnable(m_TextureMode);
}

//Fonction qui desactive le texturage
void Texture::desactivateTextureMapping()
{
	glBindTexture(m_TextureMode, 0);
	glDisable(m_TextureMode);
}

//Fonction qui active la texture courante
void Texture::activateTexture()
{
	glBindTexture(m_TextureMode, m_idTex);
}

GLuint Texture::getIdTex()
{
	return m_idTex;
}

void Texture::activateMultiTex(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	activateTextureMapping();
	activateTexture();
}

void Texture::desactivateMultiTex(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	desactivateTextureMapping();
	glActiveTexture(GL_TEXTURE0);
}

TTexturePtr Texture::LoadFromFile(const std::string& filename)
{
	TTexturePtr Resource = CResourceManager::Instance().Get<Texture>(filename);
	if (Resource == NULL)
	{
		Resource = CMediaManager::Instance().LoadMediaFromFile<Texture>(
				filename);
		CResourceManager::Instance().Add(filename, Resource);
	}
	return Resource;
}
}
