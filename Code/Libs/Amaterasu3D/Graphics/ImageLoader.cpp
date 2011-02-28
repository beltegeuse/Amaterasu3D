/*
 * ImageLoader.cpp
 *
 *  Created on: May 2, 2010
 *      Author: beltegeuse
 */

#include "ImageLoader.h"

#include <Logger/Logger.h>

ImageLoader ImageLoader::m_instance;

ImageLoadResult::ImageLoadResult() :
	success(false),
	width(0),
	height(0),
	data(NULL)
{
}

ImageLoader::ImageLoader()
{
	ilInit();
	iluInit();
}

ImageLoader::~ImageLoader()
{
	ilShutDown();
}

//FIXME: Add exception
ImageLoadResult ImageLoader::LoadFromFile(const std::string& filename)
{
	// Pour connaitre le resultat
	ImageLoadResult res;

	 /* Generation of one image name */
	ilGenImages(1, &res.texid);
	/* Binding of image name */
	ilBindImage(res.texid);

	ILenum type;
	ILboolean success = ilLoadImage(filename.c_str());


	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (success)
		{
			// Maintenant on recupere toutes les infos
			res.width =  ilGetInteger(IL_IMAGE_WIDTH);
			res.height =  ilGetInteger(IL_IMAGE_HEIGHT);
			res.data = ilGetData();
			res.success = true;
			return res;
		}
		else
		{
			Logger::Log() << "[ImageLoader] Impossible de convertir : " << filename << "\n";
		}
	}
	else
	{
		Logger::Log() << "[ImageLoader] Impossible d'ouvrir ou lire l'image : " << filename << "\n";
	}

	ILenum Error;
	while ((Error = ilGetError()) != IL_NO_ERROR) {
	Logger::Log() << "[ImageLoader] Error : " << iluErrorString(Error) << "\n";
	}
	return res;
}

void ImageLoader::DeleteImage(ImageLoadResult& res)
{
	 ilBindImage(0);
	 ilDeleteImages(1, &res.texid);
}
