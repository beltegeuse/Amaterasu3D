/*
 * WindowImpl.h
 *
 *  Created on: Apr 17, 2011
 *      Author: Beltegeuse
 */

#ifndef WINDOWIMPL_H_
#define WINDOWIMPL_H_

#include <string>
#include <WindowMode.h>

class WindowImpl {
public:
	virtual ~WindowImpl() {}

	static WindowImpl* Create(const WindowMode& mode, const std::string& name);

	virtual void Display() = 0;
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
protected:
	WindowImpl() {}

	/*
	 * Attributes
	 */
	int m_Width;
	int m_Height;
};

#endif /* WINDOWIMPL_H_ */
