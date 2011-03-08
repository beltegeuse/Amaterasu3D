/*
 * ImageLoader.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Beltegeuse
 */

#include "ImageLoader.h"
#include <Utilities/StringUtils.h>
ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

Texture* ImageLoader::LoadFromFile(const std::string& Filename)
{
	CFile file = Filename;
	const std::string ext = ToLower(file.Extension());
	if(ext == "hdr" || ext == "exr")
		return LoadHDRImageFromFile(Filename);
	else
		return LoadImageFromFile(Filename);
}

Texture* ImageLoader::LoadHDRImageFromFile(const std::string& Filename)
{
	throw CException("Unable to load HDR image format.");
	return NULL;
}

Texture* ImageLoader::LoadImageFromFile(const std::string& Filename)
{
	throw CException("Unable to load image format.");
	return NULL;
}
