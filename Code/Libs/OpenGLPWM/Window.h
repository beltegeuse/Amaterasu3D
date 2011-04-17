#ifndef WindowGL_H_
#define WindowGL_H_

// STL include
#include <string>

#include <WindowMode.h>
#include <OpenGLContextSettings.h>
#include <WindowImpl.h>

class Window {
protected:
	/*
	 * Attributes
	 */
	// To know if the windows is destroy
	WindowImpl* m_Window;
public:
	/*
	 * Constructors and destructors
	 */
	Window();
	virtual ~Window();

	/*
	 * Public methods
	 */
	// Create OpenGL window
	void Create(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings = OpenGLContextSettings());
	// To know if the window is close
	bool IsOpened() const
	{ return m_Window != NULL; }

	/*
	 * pure virtual public methods
	 */
	void Display();
};

#endif /* ABSTRACTWINDOW_H_ */
