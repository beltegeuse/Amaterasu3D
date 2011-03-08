#include <GL/glew.h>
#include <GL/gl.h>

#include "Texture.h"

#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <System/MediaManager.h>
#include <System/ResourceManager.h>

//Constructeur
Texture::Texture() :
	m_image_size(Math::TVector2I(0,0))
{
}

//Destructeur
Texture::~Texture()
{
	glDeleteTextures(1, &m_idTex);
}

//Fonction qui active le texturage
void Texture::activateTextureMapping()
{
	glEnable(GL_TEXTURE_2D);
}

//Fonction qui desactive le texturage
void Texture::desactivateTextureMapping()
{
	glDisable(GL_TEXTURE_2D);
}

//Fonction qui active la texture courante
void Texture::activateTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_idTex);
}

//Getter
int Texture::getTailleX() const
{
	return m_image_size.x;
}

int Texture::getTailleY() const
{
	return m_image_size.y;
}

GLuint* Texture::getIdTex()
{
	return &m_idTex;
}

void Texture::activateMultiTex(GLenum tex)
{
	if (glewGetExtension("GL_ARB_multitexture")){
		glActiveTextureARB(tex);
		activateTextureMapping();
		activateTexture();
	}
}

void Texture::desactivateMultiTex(GLenum tex)
{
	glActiveTextureARB(tex);
	glDisable(GL_TEXTURE_2D);
}

TTexturePtr Texture::LoadFromFile(const std::string& filename)
{
	TTexturePtr Resource = CResourceManager::Instance().Get<Texture>(filename);
	if(Resource == NULL)
	{
		Resource = CMediaManager::Instance().LoadMediaFromFile<Texture>(filename);
		CResourceManager::Instance().Add(filename, Resource);
	}
	return Resource;
}


void Texture::SetSize(const Math::TVector2I& dim)
{
	m_image_size = dim;
}

/*
 * LDRTexture
 */

LDRTexture::LDRTexture() :
		m_buffer(NULL)
{

}

LDRTexture::~LDRTexture()
{
	if(m_buffer)
		delete[] m_buffer;
}

void LDRTexture::CreateTexture(bool smooth)
{
	/* Parametrage du placage de textures */
	glGenTextures(1,&m_idTex);

	glBindTexture(GL_TEXTURE_2D,m_idTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST); //GL_LINEAR_MIPMAP_LINEAR

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, getTailleX(), getTailleY(),
			GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
}

void LDRTexture::AttachBuffer(unsigned int* buffer)
{
	m_buffer = buffer;
}
