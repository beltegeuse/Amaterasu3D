#include "WindowImplWin32.h"
#include <Exceptions.h>
#include <GL/glew.h>
#include <GL/wglew.h>

const char*  ClsName = "OpenGLPWM";

WindowImplWin32::WindowImplWin32(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings) :
		m_Handle(NULL), m_hInstance(NULL), m_DeviceContext(NULL), m_OpenGLContext(NULL)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)mode.Width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)mode.Height;		// Set Bottom Value To Requested Height

	bool fullscreen=mode.Fullscreen;			// Set The Global Fullscreen Flag

	m_hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= &WindowImplWin32::GlobalOnEvent;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= m_hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= ClsName;								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		throw new CException("Failed To Register The Window Class.");
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= mode.Width;			// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= mode.Height;			// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= mode.BitsPerPixels;	// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				throw new CException("Program Will Now Close.");
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(m_Handle=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								ClsName,							// Class Name
								name.c_str(),								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								m_hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		DestroyOpenGLWindow();
		throw new CException("Window Creation Error.");
	}

	/*
	 * OpenGL context creation
	 */

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		mode.BitsPerPixels,							// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		settings.DepthBits,							// Bit Z-Buffer (Depth Buffer)
		settings.StentilBits,						// Bit Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(m_DeviceContext=GetDC(m_Handle)))							// Did We Get A Device Context?
	{
		DestroyOpenGLWindow();								// Reset The Display
		throw new CException("Can't Create A GL Device Context.");
	}

	if (!(PixelFormat=ChoosePixelFormat(m_DeviceContext,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		DestroyOpenGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	if(!SetPixelFormat(m_DeviceContext,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		DestroyOpenGLWindow();	// Reset The Display
		throw new CException("Can't Set The PixelFormat.");
	}

	HGLRC tempContext;
	if (!(m_OpenGLContext=wglCreateContext(m_DeviceContext)))				// Are We Able To Get A Rendering Context?
	{
		DestroyOpenGLWindow();								// Reset The Display
		throw new CException("Can't Create A temporary GL Rendering Context.");
	}
	tempContext = m_OpenGLContext;

	if(!wglMakeCurrent(m_DeviceContext,tempContext))					// Try To Activate The Rendering Context
	{
		DestroyOpenGLWindow();								// Reset The Display
		throw new CException("Can't Activate The temporary GL Rendering Context.");
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		throw new CException("Impossible to initialize GLEW.");
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, settings.MajorVersion,
		WGL_CONTEXT_MINOR_VERSION_ARB, settings.MinorVersion,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	if(wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		m_OpenGLContext = wglCreateContextAttribsARB(m_DeviceContext,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(m_DeviceContext, m_OpenGLContext);
	}
	else
	{
		throw CException("Impossible to settings in the current context.");
	}

	ShowWindow(m_Handle,SW_SHOW);						// Show The Window
	SetForegroundWindow(m_Handle);						// Slightly Higher Priority
	SetFocus(m_Handle);									// Sets Keyboard Focus To The Window
}

WindowImplWin32::~WindowImplWin32() {
	DestroyOpenGLWindow();
}

LRESULT CALLBACK WindowImplWin32::GlobalOnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(handle,message,wParam,lParam);
}

void WindowImplWin32::Display()
{
	SwapBuffers(m_DeviceContext);
}

void WindowImplWin32::DestroyOpenGLWindow()
{
	if (m_OpenGLContext)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(m_OpenGLContext))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_OpenGLContext=NULL;										// Set RC To NULL
	}

	if (m_OpenGLContext && !ReleaseDC(m_Handle,m_DeviceContext))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_Handle=NULL;										// Set DC To NULL
	}

	if (m_Handle && !DestroyWindow(m_Handle))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_Handle=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",m_hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_hInstance=NULL;									// Set hInstance To NULL
	}
}
