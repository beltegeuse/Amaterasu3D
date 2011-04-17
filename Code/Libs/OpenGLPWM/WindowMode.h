/*
 * WindowMode.h
 *
 *  Created on: Apr 17, 2011
 *      Author: Beltegeuse
 */

#ifndef WINDOWMODE_H_
#define WINDOWMODE_H_

struct WindowMode
{
	WindowMode(int VWidth, int VHeight, bool fullScreen = false, int bits = 32) :
		Width(VWidth), Height(VHeight), Fullscreen(fullScreen), BitsPerPixels(bits)
	{}
	int Width;
	int Height;
	bool Fullscreen;
	int BitsPerPixels;
};

#endif /* WINDOWMODE_H_ */
