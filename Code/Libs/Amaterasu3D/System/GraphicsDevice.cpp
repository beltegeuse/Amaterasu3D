#include "GraphicsDevice.h"
#include <GL/glew.h>
#include <iostream> //#include <Logger/Logger.h>

namespace ama3D
{

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
	if (initialisationGLEW != GLEW_OK)
	{
		// Print the error
		std::cout << "[Error] Can't initialize GLEW : "
				<< glewGetErrorString(initialisationGLEW) << "\n";
		throw CException("Can't initialize GLEW.");
	}
	std::cout << "\n";
	std::cout << "====== GraphicsDevice::CheckSystem ======= \n";
	std::cout << " *** CG informations : \n";
	std::cout << "    - OpenGL Vendor: " << (char*) glGetString(GL_VENDOR)
			<< "\n";
	std::cout << "    - OpenGL Renderer: "
			<< (char*) glGetString(GL_RENDERER) << "\n";
	std::cout << "    - OpenGL Version: " << (char*) glGetString(GL_VERSION)
			<< "\n\n";

#ifdef WIN32
	if (glewIsSupported("GL_VERSION_3_1"))
	{
		std::cout << "[INFO] : Ready for OpenGL 3.1\n";
	}
	else // Fatal error
	{
		throw GraphicsDeviceException("OpenGL 3.1 is not supported.");
	}
#endif

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader
			&& GL_EXT_geometry_shader4
		)
		std::cout
				<< "[INFO] Ready for GLSL - vertex, fragment, and geometry units\n";
	else
	{
		if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
			throw GraphicsDeviceException("Shader function isn't supported.");
		else
			throw GraphicsDeviceException(
					"Geometry Shader function isn't supported.");
	}

	std::cout << "====== GraphicsDevice::CheckSystem (End) ======= \n";
	std::cout << "\n";
}

}

