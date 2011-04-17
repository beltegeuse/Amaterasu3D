/*
 * OpenGLContextSettings.h
 *
 *  Created on: Apr 17, 2011
 *      Author: Beltegeuse
 */

#ifndef OPENGLCONTEXTSETTINGS_H_
#define OPENGLCONTEXTSETTINGS_H_

struct OpenGLContextSettings
{
	OpenGLContextSettings(int majV = 3, int minV = 1, int depth = 24, int stentil = 8 ) :
		MajorVersion(majV),
		MinorVersion(minV),
		DepthBits(depth),
		StentilBits(stentil)
	{}

	int MajorVersion;
	int MinorVersion;
	int DepthBits;
	int StentilBits;
};

#endif /* OPENGLCONTEXTSETTINGS_H_ */
