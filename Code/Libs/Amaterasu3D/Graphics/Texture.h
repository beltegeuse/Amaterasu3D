#ifndef __Texture_h_
#define __Texture_h_
#include <GL/glew.h>
#include <GL/gl.h>
#include <Math/Vector2.h>
#include <System/Resource.h>
#include <Utilities/SmartPtr.h>

//Class Texture
class Texture : public IResource
{

protected:
	// ======== Attributs
	//Pointeur sur la texture
	GLuint m_idTex;
	//Donnees
	Math::TVector2I m_image_size;
	float* m_texture_data;

public :

	// ======== Constructeurs & Destructeur
	Texture();
	virtual ~Texture();

	// Static methods to load textures
	static CSmartPtr<Texture, CResourceCOM> LoadFromFile(const std::string& name);

	// ======== OpenGL methodes
	//   * Pour les methodes normales
	//Fonction qui active le texturage
	void activateTextureMapping();
	//Fonction qui desactive le texturage
	void desactivateTextureMapping();
	//Fonction qui active la texture courante
	void activateTexture();

	//    * Pour les methodes multitextures
	void activateMultiTex(int texUnit);
	void desactivateMultiTex(int texUnit);

	// ======== Get information
	int getTailleX() const;
	int getTailleY() const;
	unsigned int* getIdTex();

	// Setters
	void SetSize(const Math::TVector2I& dim);

	// Virtual function
	virtual void CreateTexture(bool smooth) = 0;
};

class LDRTexture : public Texture
{
protected:
	GLubyte* m_buffer;
public:
	LDRTexture();
	virtual ~LDRTexture();

	void AttachBuffer(GLubyte* buffer);
	virtual void CreateTexture(bool smooth);
};

typedef CSmartPtr<Texture, CResourceCOM> TTexturePtr;


#endif
