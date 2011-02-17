#include "Texture.h"

#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <CoreEngine/Managers/GameConfigManager.h>

//Constructeur
Texture::Texture(const std::string& path, bool smooth)
{
	Logger::Log() << "[Texture] Chargement de " << path << "\n";

	m_image = ImageLoader::LoadFromFile(path);

	if(!m_image.success)
	{
		throw CLoadingFailed(m_filename,"[Texture] Impossible de charger l'image !");
	}

	CreateTexture(smooth);
}

//Destructeur
Texture::~Texture()
{
	if(m_image.success)
	{
		ImageLoader::DeleteImage(m_image);
	}

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
std::string Texture::getFilename()
{
	return m_filename;
}

int Texture::getTailleX() const
{
	return m_image.width;
}

int Texture::getTailleY() const
{
	return  m_image.height;
}

GLuint* Texture::getIdTex()
{
	return &m_idTex;
}


void Texture::CreateTexture(bool smooth)
{
	/* Parametrage du placage de textures */
	glGenTextures(1,&m_idTex);

	glBindTexture(GL_TEXTURE_2D,m_idTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST); //GL_LINEAR_MIPMAP_LINEAR

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, getTailleX(), getTailleY(),
			GL_RGBA, GL_UNSIGNED_BYTE, m_image.data);
}

sf::Color Texture::GetPixel(unsigned int x, unsigned y) const
{
	sf::Color* t = reinterpret_cast<sf::Color*>(m_image.data);
	return (t[x + getTailleY()*y]);
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
