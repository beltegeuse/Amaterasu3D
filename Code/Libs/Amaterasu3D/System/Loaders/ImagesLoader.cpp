/*
 * ImageLoader.cpp
 *
 *  Created on: Mar 7, 2011
 *      Author: Beltegeuse
 */

#include "ImagesLoader.h"
#include <Utilities/StringUtils.h>
#include <Utilities/File.h>

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
	throw CException("Unable to load image format.");
	return NULL;
}
