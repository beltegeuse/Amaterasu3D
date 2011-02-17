#ifndef __Texture_h_
#define __Texture_h_

//Classe Texture

#include <GraphicsEngine/Include.h>
#include <GraphicsEngine/Utilities/ImageLoader.h>

class Texture
{

private :

	// ======== Attributs
	//Fichier texture
	std::string m_filename;
	//Pointeur sur la texture
	GLuint m_idTex;
	//Le Resultat
	ImageLoadResult m_image;

public :

	// ======== Constructeurs & Destructeur
	Texture(const std::string& filename, bool smooth = true);
	virtual ~Texture();

	// ======== OpenGL methodes
	//   * Pour les methodes normales
	//Fonction qui active le texturage
	void activateTextureMapping();
	//Fonction qui desactive le texturage
	void desactivateTextureMapping();
	//Fonction qui active la texture courante
	void activateTexture();

	//    * Pour les methodes multitextures
	void activateMultiTex(GLenum tex);
	void desactivateMultiTex(GLenum tex);

	// ======== Get information
	std::string getFilename();
	int getTailleX() const;
	int getTailleY() const;
	unsigned int* getIdTex();

	// ======== Pour modifier la texture
	sf::Color GetPixel(unsigned int x, unsigned y) const;
	void SetPixel(unsigned int x, unsigned y, const sf::Color& color);

private:
	void CreateTexture(bool smooth);

};


#endif
