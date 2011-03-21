/*
 * ImageLoader.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Beltegeuse
 */

#include "ImagesLoader.h"
#include <Utilities/StringUtils.h>
#include <Utilities/File.h>
#include <FreeImage.h>
#include <Logger/Logger.h>

ImagesLoader::ImagesLoader()
{
}

ImagesLoader::~ImagesLoader()
{
}

Texture* ImagesLoader::LoadFromFile(const std::string& Filename)
{
	CFile file = Filename;
	const std::string ext = ToLower(file.Extension());
	if(ext == "hdr" || ext == "exr")
		return LoadHDRImageFromFile(Filename);
	else
		return LoadImageFromFile(Filename);
}

Texture* ImagesLoader::LoadHDRImageFromFile(const std::string& Filename)
{
	throw CException("Unable to load HDR image format.");
	return NULL;
}

Texture* ImagesLoader::LoadImageFromFile(const std::string& Filename)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(Filename.c_str(),0);//Automatocally detects the format(from over 20 formats!)
	FIBITMAP* imagen = FreeImage_Load(formato, Filename.c_str());

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);
	Logger::Log() <<"The size of the image is: "<<Filename<<" es "<<w<<"*"<<h<<"\n"; //Some debugging code

	GLubyte* textura = new GLubyte[4*w*h];
	char* pixeles = (char*)FreeImage_GetBits(imagen);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	for(int j= 0; j<w*h; j++){
		textura[j*4+0]= pixeles[j*4+2];
		textura[j*4+1]= pixeles[j*4+1];
		textura[j*4+2]= pixeles[j*4+0];
		textura[j*4+3]= pixeles[j*4+3];
	}


	// Send all information in TextureFile
	Texture* texture = new Texture(Math::TVector2I(h,w));
	TextureMipmapsParams param;
	glBindTexture(GL_TEXTURE_2D,texture->getIdTex());
	param.applyParam();
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texture->getTailleY(), texture->getTailleX(),
					GL_RGBA, GL_UNSIGNED_BYTE, textura);

	FreeImage_Unload(imagen);
	glBindTexture(GL_TEXTURE_2D,0);
	delete[] textura;
	return texture;
}
