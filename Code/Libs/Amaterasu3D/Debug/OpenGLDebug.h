#ifndef OPENGLDEBUG_H_
#define OPENGLDEBUG_H_

#include <string>

#ifdef DEBUG
    // In debug mode, perform a test on every OpenGL call
    #define GLCheck(Func) ((Func), CheckGLError(__FILE__, __LINE__))
	#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)
#else
    // Else, we don't add any overhead
    #define GLCheck(Func) (Func)
    #define CHECK_GL_ERROR()
#endif


// Gestion des erreurs OpengL
int CheckGLError(std::string file, int line);

#endif /* OPENGLDEBUG_H_ */
