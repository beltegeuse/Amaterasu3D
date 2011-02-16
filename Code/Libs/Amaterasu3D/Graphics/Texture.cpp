#include <Graphics/Texture.h>

//Constructeur
Texture::Texture(const std::string& filename)
{
	this->filename = filename;

	this->imageTex.LoadFromFile(filename);

	this->tailleX = imageTex.GetWidth();
	this->tailleZ = imageTex.GetHeight();

	/* Parametrage du placage de textures */
  
	glGenTextures(1,IdTex);

	glBindTexture(GL_TEXTURE_2D,IdTex[0]);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,imageTex.GetWidth(),imageTex.GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,imageTex.GetPixelsPtr());

}

Texture::Texture(unsigned char* image_tex, int tailleX, int tailleZ)
{
	this->filename = "";

	this->tailleX = tailleX;
	this->tailleZ = tailleZ;

	/* Parametrage du placage de textures */

	glGenTextures(1,IdTex);

	glBindTexture(GL_TEXTURE_2D,IdTex[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tailleX,tailleZ,0,GL_RGBA,GL_UNSIGNED_BYTE,image_tex);

}

//Destructeur
Texture::~Texture()
{
}

//Fonction qui active le texturage
void Texture::activateTextureMapping()
{
	glEnable(GL_TEXTURE_2D);
}

//Fonction qui d�sactive le texturage
void Texture::desactivateTextureMapping()
{
	glDisable(GL_TEXTURE_2D);
}

//Fonction qui active la texture courante
void Texture::activateTexture()
{
	glBindTexture(GL_TEXTURE_2D, IdTex[0]);
}

//Getter
std::string Texture::getFilename()
{
	return filename;
}

int Texture::getTailleX()
{
	return tailleX;
}

int Texture::getTailleY()
{
	return tailleZ;
}

unsigned int* Texture::getIdTex()
{
	return IdTex;
}

sf::Image Texture::getTable()	//Fonction qui retourne le tableau de l'image
{
	return imageTex;
}
