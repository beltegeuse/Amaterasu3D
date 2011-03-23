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
#ifndef __Texture_h_
#define __Texture_h_
#include <GL/glew.h>
#include <GL/gl.h>
#include <Math/Vector2.h>
#include <System/Resource.h>
#include <Utilities/SmartPtr.h>

class TextureParams
{
public:
	GLfloat SWrap;
	GLfloat TWrap;
	GLint MinFiltering;
	GLint MaxFiltering;
	GLint EnvMode;

	TextureParams() :
		SWrap(GL_REPEAT),
		TWrap(GL_REPEAT),
		MinFiltering(GL_LINEAR),
		MaxFiltering(GL_LINEAR),
		EnvMode(0)
	{
	}

	virtual void applyParam()
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SWrap);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MaxFiltering);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFiltering);
		if(EnvMode)
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, EnvMode);
	}
};

class TextureMipmapsParams : public TextureParams
{
public:
	TextureMipmapsParams()
	{
		MinFiltering = GL_LINEAR_MIPMAP_LINEAR;
	}
};

//Class Texture
class Texture : public IResource
{

protected:
	// ======== Attributs
	//Pointeur sur la texture
	GLuint m_idTex;
	//Donnees
	Math::TVector2I m_size;

public :

	// ======== Constructeurs & Destructeur
	Texture(const Math::TVector2I& size, bool create = true, GLuint id = 0);
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
	GLuint getIdTex();

	// Setters
	void SetSize(const Math::TVector2I& dim);

};

typedef CSmartPtr<Texture, CResourceCOM> TTexturePtr;


#endif
