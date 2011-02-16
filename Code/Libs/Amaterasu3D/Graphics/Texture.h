#ifndef __Texture_h_
#define __Texture_h_

//Classe Texture
#include <GL/glew.h>
#include <string>
#include <SFML/Graphics.hpp>


class Texture
{

private :

	//Attributs
	std::string filename; //Fichier texture
	int tailleX, tailleZ; //Tailles de la texture
	unsigned int IdTex[1]; //Pointeur sur la texture 
	sf::Image imageTex; //Image de la texture


public :

	//Constructeur
	Texture(const std::string& filename);

	Texture(unsigned char* image_tex, int tailleX, int tailleZ);

	//Destructeur
	virtual ~Texture();

	//Fonction qui active le texturage
	void activateTextureMapping();

	//Fonction qui desactive le texturage
	void desactivateTextureMapping();

	//Fonction qui active la texture courante
	void activateTexture();

	//Getter
	std::string getFilename();
	int getTailleX();
	int getTailleY();
	unsigned int* getIdTex();
	sf::Image getTable();	//Fonction qui retourne le tableau de l'image

};


#endif
