#ifndef WINDOWIMPLWIN32_H_
#define WINDOWIMPLWIN32_H_

#include <WindowImpl.h>
#include <windows.h>

class WindowImplWin32 : public WindowImpl
{
private:
	HWND m_Handle; ///< Win32 handle of the window
	HINSTANCE m_hInstance;
	HDC m_DeviceContext;
	HGLRC m_OpenGLContext;
	bool m_IsCursorIn;
	bool m_KeyRepeatEnabled;
public:
	WindowImplWin32(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings);
	virtual ~WindowImplWin32();

	void Display();
private:
	static LRESULT CALLBACK GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void DestroyOpenGLWindow();
	Key::Code VirtualKeyCodeToSF(WPARAM key, LPARAM flags);
};

#endif /* WINDOWIMPLWIN32_H_ */
