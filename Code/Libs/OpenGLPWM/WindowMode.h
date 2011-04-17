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
	WindowMode(int VWidth, int VHeight, bool fullScreen = false) :
		Width(VWidth), Height(VHeight), Fullscreen(fullScreen)
	{}
	int Width;
	int Height;
	bool Fullscreen;
};

#endif /* WINDOWMODE_H_ */
