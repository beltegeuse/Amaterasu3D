#include "OpenGLDebug.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>


int CheckGLError(std::string file, int line)
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		const GLubyte* sError = gluErrorString(glErr);

		if (sError)
			std::cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << std::endl;
		else
			std::cout << "GL Error #" << glErr << " (no message available)" << " in File " << file << " at line: " << line << std::endl;

		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

