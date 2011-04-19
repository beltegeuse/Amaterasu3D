/*
 * WindowImpl.h
 *
 *  Created on: Apr 17, 2011
 *      Author: Beltegeuse
 */

#ifndef WINDOWIMPL_H_
#define WINDOWIMPL_H_

#include <string>
#include <queue>

#include <WindowMode.h>
#include <OpenGLContextSettings.h>
#include <Events.h>
class WindowImpl {
private:
	std::queue<Event> m_Events;

public:
	// Destructors
	virtual ~WindowImpl() {}

	/*
	 * Public static methods
	 */
	static WindowImpl* Create(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings);

	/*
	 * Public pure virtual functions
	 */
	virtual void Display() = 0;

	/*
	 * Public methods
	 */
	// Get information for the size of the rendering
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	// events methods
	bool GetEvent(Event& event)
	{
		if(m_Events.empty())
			return false;
		event = m_Events.front();
		m_Events.pop();
		return true;
	}
	void PushEvent(Event& event) { m_Events.push(event); }


protected:
	WindowImpl() {}

	/*
	 * Attributes
	 */
	int m_Width;
	int m_Height;
};

#endif /* WINDOWIMPL_H_ */
