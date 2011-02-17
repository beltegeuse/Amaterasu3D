/*
 * ImageLoader.h
 *
 *  Created on: May 2, 2010
 *      Author: beltegeuse
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_


#include <string>

struct ImageLoadResult
{
	ImageLoadResult();
	bool success;
	int width;
	int height;
	unsigned char* data;
	unsigned int texid;
};

class ImageLoader {
private:
	static ImageLoader m_instance;
	//static bool m_init;
public:
	ImageLoader();
	virtual ~ImageLoader();

	static ImageLoadResult LoadFromFile(const std::string& filename);
	static void DeleteImage(ImageLoadResult& res);
private:
	static bool GetExtention(ILenum& e, const std::string& filename);
};

#endif /* IMAGELOADER_H_ */
