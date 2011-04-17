#include "WindowImpl.h"

#include <Win32/WindowImplWin32.h>
typedef WindowImplWin32 WindowImplType;

WindowImpl* WindowImpl::Create(const WindowMode& mode, const std::string& name, const OpenGLContextSettings& settings)
{
	return new WindowImplType(mode, name, settings);
}
