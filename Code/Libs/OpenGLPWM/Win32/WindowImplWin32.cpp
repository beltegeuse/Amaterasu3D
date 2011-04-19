#include "WindowImplWin32.h"
#include <Exceptions.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <iostream>

const char*  ClsName = "OpenGLPWM";

#ifndef MAPVK_VK_TO_VSC
    #define MAPVK_VK_TO_VSC (0)
#endif

WindowImplWin32::WindowImplWin32(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings) :
		m_Handle(NULL), m_hInstance(NULL), m_DeviceContext(NULL), m_OpenGLContext(NULL), m_IsCursorIn(false), m_KeyRepeatEnabled(true)
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
	if (message == WM_CREATE)
	{
		// Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
		LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

		// Set as the "user data" parameter of the window
		SetWindowLongPtr(handle, GWLP_USERDATA, window);
	}

	// Get the WindowImpl instance corresponding to the window handle
	WindowImplWin32* window = reinterpret_cast<WindowImplWin32*>(GetWindowLongPtr(handle, GWLP_USERDATA));

	// Forward the event to the appropriate function
	if (window)
	{
		window->ProcessEvent(message, wParam, lParam);
	}
	else
	{
		std::cout << "Skipping ..." << std::endl;
	}

	if (message == WM_CLOSE)
	{
		return 0;
	}

	return DefWindowProc(handle,message,wParam,lParam);
}

void WindowImplWin32::ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Don't process any message until window is created
	if (m_Handle == NULL)
		return;

	switch (message)
	{
		// Destroy event
		case WM_DESTROY :
		{
			// Here we must cleanup resources !
			DestroyOpenGLWindow();
			break;
		}

		// Set cursor event
		case WM_SETCURSOR :
		{
			//FIXME
			// The mouse has moved, if the cursor is in our window we must refresh the cursor
			//if (LOWORD(lParam) == HTCLIENT)
			//	SetCursor(myCursor);

			break;
		}

		// Close event
		case WM_CLOSE :
		{
			Event event;
			event.Type = Event::Closed;
			PushEvent(event);
			break;
		}

		// Resize event
		case WM_SIZE :
		{
			// Ignore size events triggered by a minimize (size == 0 in this case)
			if (wParam != SIZE_MINIMIZED)
			{
				// Update window size
				RECT rectangle;
				GetClientRect(m_Handle, &rectangle);
				m_Width  = rectangle.right - rectangle.left;
				m_Height = rectangle.bottom - rectangle.top;

				Event event;
				event.Type        = Event::Resized;
				event.Size.Width  = m_Width;
				event.Size.Height = m_Height;
				PushEvent(event);
				break;
			}
		}

		// Gain focus event
		case WM_SETFOCUS :
		{
			Event event;
			event.Type = Event::GainedFocus;
			PushEvent(event);
			break;
		}

		// Lost focus event
		case WM_KILLFOCUS :
		{
			Event event;
			event.Type = Event::LostFocus;
			PushEvent(event);
			break;
		}

		// Text event
		case WM_CHAR :
		{
			if (m_KeyRepeatEnabled || ((lParam & (1 << 30)) == 0))
			{
				Event event;
				event.Type = Event::TextEntered;
				event.Text.Character = static_cast<char>(wParam);
				PushEvent(event);
			}
			break;
		}

		// Keydown event
		case WM_KEYDOWN :
		case WM_SYSKEYDOWN :
		{
			if (m_KeyRepeatEnabled || ((HIWORD(lParam) & KF_REPEAT) == 0))
			{
				Event event;
				event.Type        = Event::KeyPressed;
				event.Key.Alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
				event.Key.Control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
				event.Key.Shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
				event.Key.Code    = VirtualKeyCodeToSF(wParam, lParam);
				PushEvent(event);
			}
			break;
		}

		// Keyup event
		case WM_KEYUP :
		case WM_SYSKEYUP :
		{
			Event event;
			event.Type        = Event::KeyReleased;
			event.Key.Alt     = HIWORD(GetAsyncKeyState(VK_MENU))    != 0;
			event.Key.Control = HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0;
			event.Key.Shift   = HIWORD(GetAsyncKeyState(VK_SHIFT))   != 0;
			event.Key.Code    = VirtualKeyCodeToSF(wParam, lParam);
			PushEvent(event);
			break;
		}

		// Mouse wheel event
		case WM_MOUSEWHEEL :
		{
			// Mouse position is in screen coordinates, convert it to window coordinates
			POINT position;
			position.x = static_cast<int>(LOWORD(lParam));
			position.y = static_cast<int>(HIWORD(lParam));
			ScreenToClient(m_Handle, &position);

			Event event;
			event.Type = Event::MouseWheelMoved;
			event.MouseWheel.Delta = static_cast<int>(HIWORD(wParam)) / 120;
			event.MouseButton.X    = position.x;
			event.MouseButton.Y    = position.y;
			PushEvent(event);
			break;
		}

		// Mouse left button down event
		case WM_LBUTTONDOWN :
		{
			Event event;
			event.Type               = Event::MouseButtonPressed;
			event.MouseButton.Button = Mouse::Left;
			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		// Mouse left button up event
		case WM_LBUTTONUP :
		{
			Event event;
			event.Type               = Event::MouseButtonReleased;
			event.MouseButton.Button = Mouse::Left;
			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		// Mouse right button down event
		case WM_RBUTTONDOWN :
		{
			Event event;
			event.Type               = Event::MouseButtonPressed;
			event.MouseButton.Button = Mouse::Right;
			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		// Mouse right button up event
		case WM_RBUTTONUP :
		{
			Event event;
			event.Type               = Event::MouseButtonReleased;
			event.MouseButton.Button = Mouse::Right;
			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		// Mouse wheel button down event
		case WM_MBUTTONDOWN :
		{
			Event event;
			event.Type               = Event::MouseButtonPressed;
			event.MouseButton.Button = Mouse::Middle;
			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		// Mouse wheel button up event
		case WM_MBUTTONUP :
		{
			Event event;
			event.Type               = Event::MouseButtonReleased;
			event.MouseButton.Button = Mouse::Middle;
			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

//		// Mouse X button down event
//		case WM_XBUTTONDOWN :
//		{
//			Event event;
//			event.Type               = Event::MouseButtonPressed;
//			event.MouseButton.Button = HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
//			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
//			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
//			PushEvent(event);
//			break;
//		}
//
//		// Mouse X button up event
//		case WM_XBUTTONUP :
//		{
//			Event event;
//			event.Type               = Event::MouseButtonReleased;
//			event.MouseButton.Button = HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2;
//			event.MouseButton.X      = static_cast<int>(LOWORD(lParam));
//			event.MouseButton.Y      = static_cast<int>(HIWORD(lParam));
//			PushEvent(event);
//			break;
//		}

		// Mouse move event
		case WM_MOUSEMOVE :
		{
			// Check if we need to generate a MouseEntered event
			if (!m_IsCursorIn)
			{
				TRACKMOUSEEVENT mouseEvent;
				mouseEvent.cbSize    = sizeof(TRACKMOUSEEVENT);
				mouseEvent.hwndTrack = m_Handle;
				mouseEvent.dwFlags   = TME_LEAVE;
				TrackMouseEvent(&mouseEvent);

				m_IsCursorIn = true;

				Event event;
				event.Type = Event::MouseEntered;
				PushEvent(event);
			}

			Event event;
			event.Type        = Event::MouseMoved;
			event.MouseMove.X = static_cast<int>(LOWORD(lParam));
			event.MouseMove.Y = static_cast<int>(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		// Mouse leave event
		case WM_MOUSELEAVE :
		{
			m_IsCursorIn = false;

			Event event;
			event.Type = Event::MouseLeft;
			PushEvent(event);
			break;
		}
	}
}

Key::Code WindowImplWin32::VirtualKeyCodeToSF(WPARAM key, LPARAM flags)
{
    switch (key)
    {
        // Check the scancode to distinguish between left and right shift
        case VK_SHIFT :
        {
            static UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
            return scancode == lShift ? Key::LShift : Key::RShift;
        }

        // Check the "extended" flag to distinguish between left and right alt
        case VK_MENU : return (HIWORD(flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;

        // Check the "extended" flag to distinguish between left and right control
        case VK_CONTROL : return (HIWORD(flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;

        // Other keys are reported properly
        case VK_LWIN :       return Key::LSystem;
        case VK_RWIN :       return Key::RSystem;
        case VK_APPS :       return Key::Menu;
        case VK_OEM_1 :      return Key::SemiColon;
        case VK_OEM_2 :      return Key::Slash;
        //case VK_OEM_PLUS :   return Key::Equal;
        //case VK_OEM_MINUS :  return Key::Dash;
        case VK_OEM_4 :      return Key::LBracket;
        case VK_OEM_6 :      return Key::RBracket;
        //case VK_OEM_COMMA :  return Key::Comma;
        //case VK_OEM_PERIOD : return Key::Period;
        case VK_OEM_7 :      return Key::Quote;
        case VK_OEM_5 :      return Key::BackSlash;
        case VK_OEM_3 :      return Key::Tilde;
        case VK_ESCAPE :     return Key::Escape;
        case VK_SPACE :      return Key::Space;
        case VK_RETURN :     return Key::Return;
        case VK_BACK :       return Key::Back;
        case VK_TAB :        return Key::Tab;
        case VK_PRIOR :      return Key::PageUp;
        case VK_NEXT :       return Key::PageDown;
        case VK_END :        return Key::End;
        case VK_HOME :       return Key::Home;
        case VK_INSERT :     return Key::Insert;
        case VK_DELETE :     return Key::Delete;
        case VK_ADD :        return Key::Add;
        case VK_SUBTRACT :   return Key::Subtract;
        case VK_MULTIPLY :   return Key::Multiply;
        case VK_DIVIDE :     return Key::Divide;
        case VK_PAUSE :      return Key::Pause;
        case VK_F1 :         return Key::F1;
        case VK_F2 :         return Key::F2;
        case VK_F3 :         return Key::F3;
        case VK_F4 :         return Key::F4;
        case VK_F5 :         return Key::F5;
        case VK_F6 :         return Key::F6;
        case VK_F7 :         return Key::F7;
        case VK_F8 :         return Key::F8;
        case VK_F9 :         return Key::F9;
        case VK_F10 :        return Key::F10;
        case VK_F11 :        return Key::F11;
        case VK_F12 :        return Key::F12;
        case VK_F13 :        return Key::F13;
        case VK_F14 :        return Key::F14;
        case VK_F15 :        return Key::F15;
        case VK_LEFT :       return Key::Left;
        case VK_RIGHT :      return Key::Right;
        case VK_UP :         return Key::Up;
        case VK_DOWN :       return Key::Down;
        case VK_NUMPAD0 :    return Key::Numpad0;
        case VK_NUMPAD1 :    return Key::Numpad1;
        case VK_NUMPAD2 :    return Key::Numpad2;
        case VK_NUMPAD3 :    return Key::Numpad3;
        case VK_NUMPAD4 :    return Key::Numpad4;
        case VK_NUMPAD5 :    return Key::Numpad5;
        case VK_NUMPAD6 :    return Key::Numpad6;
        case VK_NUMPAD7 :    return Key::Numpad7;
        case VK_NUMPAD8 :    return Key::Numpad8;
        case VK_NUMPAD9 :    return Key::Numpad9;
        case 'A' :           return Key::A;
        case 'Z' :           return Key::Z;
        case 'E' :           return Key::E;
        case 'R' :           return Key::R;
        case 'T' :           return Key::T;
        case 'Y' :           return Key::Y;
        case 'U' :           return Key::U;
        case 'I' :           return Key::I;
        case 'O' :           return Key::O;
        case 'P' :           return Key::P;
        case 'Q' :           return Key::Q;
        case 'S' :           return Key::S;
        case 'D' :           return Key::D;
        case 'F' :           return Key::F;
        case 'G' :           return Key::G;
        case 'H' :           return Key::H;
        case 'J' :           return Key::J;
        case 'K' :           return Key::K;
        case 'L' :           return Key::L;
        case 'M' :           return Key::M;
        case 'W' :           return Key::W;
        case 'X' :           return Key::X;
        case 'C' :           return Key::C;
        case 'V' :           return Key::V;
        case 'B' :           return Key::B;
        case 'N' :           return Key::N;
        case '0' :           return Key::Num0;
        case '1' :           return Key::Num1;
        case '2' :           return Key::Num2;
        case '3' :           return Key::Num3;
        case '4' :           return Key::Num4;
        case '5' :           return Key::Num5;
        case '6' :           return Key::Num6;
        case '7' :           return Key::Num7;
        case '8' :           return Key::Num8;
        case '9' :           return Key::Num9;
    }

    return Key::Code(0);
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
			throw CException("Release Of DC And RC Failed.");
		}

		if (!wglDeleteContext(m_OpenGLContext))						// Are We Able To Delete The RC?
		{
			throw CException("Release Rendering Context Failed.");
		}
		m_OpenGLContext=NULL;										// Set RC To NULL
	}

	if (m_OpenGLContext && !ReleaseDC(m_Handle,m_DeviceContext))					// Are We Able To Release The DC
	{
		throw CException("Release Device Context Failed.");
		m_Handle=NULL;										// Set DC To NULL
	}

	if (m_Handle && !DestroyWindow(m_Handle))					// Are We Able To Destroy The Window?
	{
		throw CException("Could Not Release hWnd.");
		m_Handle=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",m_hInstance))			// Are We Able To Unregister Class
	{
		throw CException("Could Not Unregister Class.");
		m_hInstance=NULL;									// Set hInstance To NULL
	}
}
