/*
 * ImageLoader.h
 *
 *  Created on: Mar 7, 2011
 *      Author: Beltegeuse
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_
#include <System/Loaders/Loader.h>
#include <Graphics/Texture.h>

class ImagesLoader : public ILoader<Texture>
{
public:
	ImagesLoader();
	virtual ~ImagesLoader();
	virtual Texture* LoadFromFile(const std::string& Filename);
private:
	 Texture* LoadHDRImageFromFile(const std::string& Filename);
	 Texture* LoadImageFromFile(const std::string& Filename);
};

#endif /* IMAGELOADER_H_ */