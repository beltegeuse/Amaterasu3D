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
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(Filename.c_str(), 0);

	if(fifmt == FIF_UNKNOWN)
	{
		Logger::Log() << "[Error] tpImageIO : Unknow type !\n";
		throw CException("[FreeImage] Unknow file type");
	}

	FIBITMAP *dib = FreeImage_Load(fifmt, Filename.c_str(),0);

	if( dib == NULL )
	{
		Logger::Log() << "[Error] tpImageIO : Impossible d'ouvrir l'image : " << Filename << "\n";
		throw CException("[FreeImage] unreadable image");
	}

	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(dib);

	int nbBits = (int)FreeImage_GetBPP(dib);

	Logger::Log() << "[Info] [read color] Image BPP : " <<  nbBits << "\n";

	//FIXME: Gestion de la transparence
	if((image_type != FIT_BITMAP) || (FreeImage_GetBPP(dib) != 24))
	{
		Logger::Log() << "[Error] tpImageIO (read Color): Unknow type !\n";
		throw CException("[FreeImage] Unknow bit size.");
	}

	unsigned width = FreeImage_GetWidth(dib);
	unsigned height = FreeImage_GetHeight(dib);
	unsigned pitch = FreeImage_GetPitch(dib);

	Logger::Log() << "[INFO] Create texture : " << height << "x" << width << "\n";
	unsigned int * data = new unsigned int[width*height*4];

	BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
	if((image_type == FIT_BITMAP) && (FreeImage_GetBPP(dib) == 24))
	{
		for(unsigned int y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for(unsigned int x = 0; x < width; x++) {
				data[(y*width+x)*3] = pixel[FI_RGBA_RED];
				data[(y*width+x)*3+1]= pixel[FI_RGBA_GREEN];
				data[(y*width+x)*3+2] = pixel[FI_RGBA_BLUE];
				data[(y*width+x)*3+3] = 255;
				pixel += (nbBits / 8);
			} // next line
			bits += pitch;
		}
	}
	else if(FreeImage_GetBPP(dib) == 8)
	{
		std::cout << "[Warning] Read => Force grayscale image ! " << std::endl;

		for(unsigned int y = 0; y < height; y++) {
			BYTE *pixel = (BYTE*)bits;
			for(unsigned int x = 0; x < width; x++) {
				data[(y*width+x)*3] = pixel[0];
				data[(y*width+x)*3+1] = pixel[0];
				data[(y*width+x)*3+2] = pixel[0];
				data[(y*width+x)*3+3] = pixel[0];
				pixel += 1;
			} // next line
			bits += pitch;
		}
	}
	else
	{
		throw CException("[FreeImage] Unable to load data");
	}

	// Send all information in TextureFile
	LDRTexture* texture = new LDRTexture;
	texture->AttachBuffer(data);
	texture->SetSize(Math::TVector2I(height, width));
	texture->CreateTexture(true);

	FreeImage_Unload(dib);
	return texture;
}
