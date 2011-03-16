#include <GL/glew.h>
#include <GL/gl.h>

#include "Texture.h"

#include <Debug/Exceptions.h>
#include <Logger/Logger.h>
#include <System/MediaManager.h>
#include <System/ResourceManager.h>

//Constructeur
Texture::Texture(const Math::TVector2I& size, bool create, GLuint id) :
	m_size(size)
{
	if(create)
		glGenTextures(1,&m_idTex);
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
	glEnable(GL_TEXTURE_2D);
}

//Fonction qui desactive le texturage
void Texture::desactivateTextureMapping()
{
	glBindTexture(GL_TEXTURE_2D, 0);
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
	return m_size.x;
}

int Texture::getTailleY() const
{
	return m_size.y;
}

GLuint Texture::getIdTex()
{
	return m_idTex;
}

void Texture::activateMultiTex(int unit)
{
	glActiveTexture(GL_TEXTURE0+unit);
	activateTextureMapping();
	activateTexture();
}

void Texture::desactivateMultiTex(int unit)
{
	glActiveTexture(GL_TEXTURE0+unit);
	desactivateTextureMapping();
	glActiveTexture(GL_TEXTURE0);
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
	m_size = dim;
}
