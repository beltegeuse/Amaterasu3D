#include "GraphicsDevice.h"
#include <GL/glew.h>
#include <Logger/Logger.h>

GraphicsDevice::GraphicsDevice()
{
}

GraphicsDevice::~GraphicsDevice()
{
}

void GraphicsDevice::CheckSystem()
{
	GLenum initialisationGLEW = glewInit();
	// If there is an error
	if(initialisationGLEW != GLEW_OK)
	{
		// Print the error
		Logger::Log() << "[Error] Can't initialize GLEW : " << glewGetErrorString(initialisationGLEW) << "\n";
		throw CException("Can't initialize GLEW.");
	}
	Logger::Log() << "\n";
	Logger::Log() << "====== GraphicsDevice::CheckSystem ======= \n";
	Logger::Log() << " *** CG informations : \n";
	Logger::Log() << "    - OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	Logger::Log() << "    - OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	Logger::Log() << "    - OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n\n";

	#ifdef WIN32
	if (glewIsSupported("GL_VERSION_3_1"))
	{
		Logger::Log() << "[INFO] : Ready for OpenGL 3.1\n";
	}
	else // Fatal error
	{
		throw GraphicsDeviceException("OpenGL 3.1 is not supported.");
	}
	#endif

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
		Logger::Log() << "[INFO] Ready for GLSL - vertex, fragment, and geometry units\n";
	else {
		if(!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
			throw GraphicsDeviceException("Shader function isn't supported.");
		else
			throw GraphicsDeviceException("Geometry Shader function isn't supported.");
	}

	Logger::Log() << "====== GraphicsDevice::CheckSystem (End) ======= \n";
	Logger::Log() << "\n";
}


