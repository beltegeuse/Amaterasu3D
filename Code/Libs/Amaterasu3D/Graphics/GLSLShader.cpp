/********************************************************************
glsl.cpp
Version: 1.0.0_rc5
Last update: 2006/11/12 (Geometry Shader Support)

(c) 2003-2006 by Martin Christen. All Rights reserved.
 *********************************************************************/

#include "GLSLShader.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <Logger/Logger.h>


#include <Graphics/MatrixManagement.h>
#include <System/ResourceManager.h>
#include <System/MediaManager.h>


SINGLETON_IMPL(glShaderManager)

using namespace std;

bool useGLSL = false;
bool extensions_init = false;
bool bGeometryShader = false;
bool bGPUShader4 = false;

CShaderException::CShaderException(const std::string& message) :
			CException("[ShaderException] : " + message)
{
}

//-----------------------------------------------------------------------------
/*! \mainpage
\section s_intro Introduction
This is libglsl - a collection of helper classes to load, compile, link and activate shaders
written in the OpenGL Shading language. Vertex Shaders, Geometry Shaders and Fragment shaders
are supported (if the hardware is capable of supporting them, of course).

Version info: \ref s_libglslnews

\section s_examples Examples

\subsection Loading Vertex and Fragment Shader using Shader Manager.

\verbatim
Initialization:
   glShaderManager SM;
   glShader *shader = SM.loadfromFile("test.vert","test.frag");
   if (shader==0) 
      cout << "Error Loading, compiling or linking shader\n";

Render:
   shader->begin();
   shader->setUniform1f("MyFloat", 1.123);
     glutDrawSolidSphere(1.0);
   shader->end();
\endverbatim

\subsection geom_shader Geometry Shader
The easiest way to use Geometry Shaders is through the
Shadermanager. 
Initialization:
\verbatim
 SM.SetInputPrimitiveType(GL_TRIANGLES);
 SM.SetOutputPrimitiveType(GL_TRIANGLE_STRIP);
 SM.SetVerticesOut(3);
 glShader *shader = SM.loadfromFile("test.vert","test.geom","test.frag");
\endverbatim

 */

//-----------------------------------------------------------------------------
// Error, Warning and Info Strings
char* aGLSLStrings[] = {
		"[e00] GLSL is not available!",
		"[e01] Not a valid program object!",
		"[e02] Not a valid object!",
		"[e03] Out of memory!",
		"[e04] Unknown compiler error!",
		"[e05] Linker log is not available!",
		"[e06] Compiler log is not available!",
		"[Empty]"
};
//-----------------------------------------------------------------------------      

//----------------------------------------------------------------------------- 
bool InitOpenGLExtensions(void)
{
	if (extensions_init) return true;
	extensions_init = true;

	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		cout << "Error:" << glewGetErrorString(err) << endl;
		extensions_init = false;
		return false;
	}

	cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n\n";
	//cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n\n";

	HasGLSLSupport();

	return true;
}


bool HasGLSLSupport(void)
{
	bGeometryShader = HasGeometryShaderSupport();
	bGPUShader4     = HasShaderModel4();

	if (useGLSL) return true;  // already initialized and GLSL is available
	useGLSL = true;

	if (!extensions_init) InitOpenGLExtensions();  // extensions were not yet initialized!!


	if (GLEW_VERSION_2_0)
	{
		cout << "OpenGL 2.0 (or higher) is available!" << endl;
	}
	else if (GLEW_VERSION_1_5)
	{
		cout << "OpenGL 1.5 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_4)
	{
		cout << "OpenGL 1.4 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_3)
	{
		cout << "OpenGL 1.3 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_2)
	{
		cout << "OpenGL 1.2 core functions are available" << endl;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
	{
		cout << "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
		useGLSL = false;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
	{
		cout << "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
		useGLSL = false;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
	{
		cout << "[WARNING] GL_ARB_shader_objects extension is not available!\n";
		useGLSL = false;
	}

	if (useGLSL)
	{
		cout << "[OK] OpenGL Shading Language is available!\n\n";
	}
	else
	{
		cout << "[FAILED] OpenGL Shading Language is not available...\n\n";
	}

	return useGLSL;
}


bool HasOpenGL2Support(void)
{
	if (!extensions_init) InitOpenGLExtensions();

	return (GLEW_VERSION_2_0 == GL_TRUE);
}


bool HasGeometryShaderSupport(void)
{
	if (GL_TRUE != glewGetExtension("GL_EXT_geometry_shader4"))
		return false;

	return true;
}

bool HasShaderModel4(void)
{
	if (GL_TRUE != glewGetExtension("GL_EXT_gpu_shader4"))
		return false;

	return true;
}


//----------------------------------------------------------------------------- 

// ************************************************************************
// Implementation of glShader class
// ************************************************************************

glShader::glShader()
{
	InitOpenGLExtensions();
	ProgramObject = 0;
	linker_log = 0;
	is_linked = false;
	_mM = false;
	_noshader = true;

	if (!useGLSL)
	{
		cout << "**ERROR: OpenGL Shading Language is NOT available!" << endl;
	}
	else
	{
		ProgramObject = glCreateProgram();
	}

}

//----------------------------------------------------------------------------- 

glShader::~glShader()
{
	if (linker_log!=0) free(linker_log);
	if (useGLSL)
	{
		for (unsigned int i=0;i<ShaderList.size();i++)
		{
			glDetachShader(ProgramObject, ShaderList[i]->ShaderObject);
			CHECK_GL_ERROR(); // if you get an error here, you deleted the Program object first and then
			// the ShaderObject! Always delete ShaderObjects last!
			if (_mM) delete ShaderList[i];
		}

		glDeleteShader(ProgramObject);
		CHECK_GL_ERROR();
	}

}

//----------------------------------------------------------------------------- 

void glShader::addShader(glShaderObject* ShaderProgram)
{
	if (!useGLSL) return;

	if (ShaderProgram==0) return;


	if (!ShaderProgram->is_compiled)
	{
		cout << "**warning** please compile program before adding object! trying to compile now...\n";
		if (!ShaderProgram->compile())
		{
			cout << "...compile ERROR!\n";
			return;
		}
		else
		{
			cout << "...ok!\n";
		}
	}

	ShaderList.push_back(ShaderProgram);

}

//----------------------------------------------------------------------------- 

void glShader::SetInputPrimitiveType(int nInputPrimitiveType)
{
	_nInputPrimitiveType = nInputPrimitiveType;
}

void glShader::SetOutputPrimitiveType(int nOutputPrimitiveType)
{
	_nOutputPrimitiveType = nOutputPrimitiveType;
}

void glShader::SetVerticesOut(int nVerticesOut)
{
	_nVerticesOut = nVerticesOut;
}
//----------------------------------------------------------------------------- 

bool glShader::link(void)
{
	if (!useGLSL) return false;

	unsigned int i;

	if (_bUsesGeometryShader)
	{
		glProgramParameteriEXT(ProgramObject, GL_GEOMETRY_INPUT_TYPE_EXT, _nInputPrimitiveType);
		glProgramParameteriEXT(ProgramObject, GL_GEOMETRY_OUTPUT_TYPE_EXT, _nOutputPrimitiveType);
		glProgramParameteriEXT(ProgramObject, GL_GEOMETRY_VERTICES_OUT_EXT, _nVerticesOut);
	}

	if (is_linked)  // already linked, detach everything first
	{
		cout << "**warning** Object is already linked, trying to link again" << endl;
		for (i=0;i<ShaderList.size();i++)
		{
			glDetachShader(ProgramObject, ShaderList[i]->ShaderObject);
			CHECK_GL_ERROR();
		}
	}

	for (i=0;i<ShaderList.size();i++)
	{
		glAttachShader(ProgramObject, ShaderList[i]->ShaderObject);
		CHECK_GL_ERROR();
		//cout << "attaching ProgramObj [" << i << "] @ 0x" << hex << ShaderList[i]->ProgramObject << " in ShaderObj @ 0x"  << ShaderObject << endl;
	}

	GLint linked; // bugfix Oct-06-2006
	glLinkProgram(ProgramObject);
	CHECK_GL_ERROR();
	glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
	CHECK_GL_ERROR();

	if (linked)
	{
		is_linked = true;
		return true;
	}
	else
	{
		cout << "**linker error**\n";
	}

	return false;
}

//----------------------------------------------------------------------------- 
// Compiler Log: Ausgabe der Compiler Meldungen in String

char* glShader::getLinkerLog(void)
{    
	if (!useGLSL) return aGLSLStrings[0];

	GLint blen = 0;    // bugfix Oct-06-2006
	GLsizei slen = 0;  // bugfix Oct-06-2006

	if (ProgramObject==0) return aGLSLStrings[2];
	glGetProgramiv(ProgramObject, GL_INFO_LOG_LENGTH , &blen);
	CHECK_GL_ERROR();

	if (blen > 1)
	{
		if (linker_log!=0)
		{
			free(linker_log);
			linker_log =0;
		}
		if ((linker_log = (GLcharARB*)malloc(blen)) == NULL)
		{
			printf("ERROR: Could not allocate compiler_log buffer\n");
			return aGLSLStrings[3];
		}

		glGetProgramInfoLog(ProgramObject, blen, &slen, linker_log);
		CHECK_GL_ERROR();

	}
	if (linker_log!=0)
		return (char*) linker_log;
	else
		return aGLSLStrings[5];

	return aGLSLStrings[4];
}

void glShader::begin(void)
{
	if (!useGLSL) return;
	if (ProgramObject == 0) return;
	if (!_noshader) return;

	if (is_linked)
	{
		glUseProgram(ProgramObject);
		glShaderManager::Instance().Push(this);
		CHECK_GL_ERROR();
	}
}

//----------------------------------------------------------------------------- 

void glShader::end(void)
{
	if (!useGLSL) return;
	if (!_noshader) return;

	glShaderManager::Instance().Pop();
	glUseProgram(0);
	CHECK_GL_ERROR();
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform1f(const GLcharARB* varname, GLfloat v0, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index


	glUniform1f(loc, v0);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform2f(const GLcharARB* varname, GLfloat v0, GLfloat v1, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform2f(loc, v0, v1);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform3f(const GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform3f(loc, v0, v1, v2);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform4f(const GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform4f(loc, v0, v1, v2, v3);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform1i(const GLcharARB* varname, GLint v0, GLint index)
{ 
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform1i(loc, v0);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setUniform2i(const GLcharARB* varname, GLint v0, GLint v1, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform2i(loc, v0, v1);


	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform3i(const GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform3i(loc, v0, v1, v2);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setUniform4i(const GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint v3, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform4i(loc, v0, v1, v2, v3);

	return true;
}
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------- 

bool glShader::setUniform1ui(const GLcharARB* varname, GLuint v0, GLint index)
{ 
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform1uiEXT(loc, v0);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setUniform2ui(const GLcharARB* varname, GLuint v0, GLuint v1, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform2uiEXT(loc, v0, v1);


	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform3ui(const GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform3uiEXT(loc, v0, v1, v2);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setUniform4ui(const GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform4uiEXT(loc, v0, v1, v2, v3);

	return true;
}
//-----------------------------------------------------------------------------

bool glShader::setUniform1fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform1fv(loc, count, value);

	return true;
}
bool glShader::setUniform2fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform2fv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform3fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform3fv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform4fv(const GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform4fv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform1iv(const GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform1iv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform2iv(const GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform2iv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform3iv(const GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform3iv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform4iv(const GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform4iv(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform1uiv(const GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform1uivEXT(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform2uiv(const GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform2uivEXT(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform3uiv(const GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform3uivEXT(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniform4uiv(const GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniform4uivEXT(loc, count, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniformMatrix2fv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniformMatrix2fv(loc, count, transpose, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniformMatrix3fv(const GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return false;  // can't find variable / invalid index

	glUniformMatrix3fv(loc, count, transpose, value);

	return true;
}

//----------------------------------------------------------------------------- 

bool glShader::setUniformMatrix4fv(const GLcharARB* varname, const Math::CMatrix4& matrix)
{
	GLint loc = GetUniformLocation(varname);
	glUniformMatrix4fv(loc,1, GL_TRUE, (const float*) matrix);
}

bool glShader:: setUniformMatrix3fv(const GLcharARB* varname, const Math::CMatrix3& matrix)
{
	GLint loc = GetUniformLocation(varname);
	glUniformMatrix3fv(loc,1, GL_TRUE, (const float*) matrix);
}

bool glShader::setUniformMatrix4fv(MatrixType type, const Math::CMatrix4& matrix)
{
	Assert(matrixModeAvailable(type));
	setUniformMatrix4fv(m_matrix_bind[type].c_str(), matrix);
	// TODO: Put into martix management ?
	// Need to update the NormalMatrix ...
	if(type == MODELVIEW_MATRIX && matrixModeAvailable(NORMAL_MATRIX))
	{
		Math::CMatrix4 matrixNormal;
		matrixNormal = matrix.Inverse();
		matrixNormal = matrixNormal.Transpose();
		setUniformMatrix3fv(m_matrix_bind[NORMAL_MATRIX].c_str(), matrixNormal.ExtractSubMatrix());
	}
}

//----------------------------------------------------------------------------- 

GLint glShader::GetUniformLocation(const GLcharARB *name)
{
	GLint loc;

	loc = glGetUniformLocation(ProgramObject, name);
	if (loc == -1) 
	{
		cout << "Error: can't find uniform variable \"" << name << "\"\n";
	}
	CHECK_GL_ERROR();
	return loc;
}

GLint glShader::GetAttribLocation(const GLcharARB* name)
{
	GLint loc;

	loc = glGetAttribLocation(ProgramObject, name);
	if (loc == -1)
	{
		cout << "Error: can't find uniform variable \"" << name << "\"\n";
	}
	CHECK_GL_ERROR();
	return loc;
}

//----------------------------------------------------------------------------- 

void glShader::getUniformfv(const GLcharARB* varname, GLfloat* values, GLint index)
{
	if (!useGLSL) return;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return;  // can't find variable / invalid index

	glGetUniformfv(ProgramObject, loc, values);

}



//----------------------------------------------------------------------------- 

void glShader::getUniformiv(const GLcharARB* varname, GLint* values, GLint index)
{
	if (!useGLSL) return;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return;  // can't find variable / invalid index

	glGetUniformiv(ProgramObject, loc, values);

}

//----------------------------------------------------------------------------- 

void glShader::getUniformuiv(const GLcharARB* varname, GLuint* values, GLint index)
{
	if (!useGLSL) return;

	GLint loc;
	if (varname)
		loc = GetUniformLocation(varname);
	else
		loc = index;

	if (loc==-1)
		return;  // can't find variable / invalid index

	glGetUniformuivEXT(ProgramObject, loc, values);

}

//-----------------------------------------------------------------------------
void  glShader::BindAttribLocation(GLint index, const GLchar* name)
{
	glBindAttribLocation(ProgramObject, index, name);
}

//-----------------------------------------------------------------------------


bool glShader::setVertexAttrib1f(GLuint index, GLfloat v0)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib1f(index, v0);

	return true;
}

bool glShader::setVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib2f(index, v0, v1);

	return true;
}

bool glShader::setVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib3f(index, v0, v1, v2);

	return true;
}

bool glShader::setVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib4f(index, v0, v1, v2, v3);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib1d(GLuint index, GLdouble v0)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib1d(index, v0);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib2d(index, v0, v1);

	return true;
}

//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib3d(index, v0, v1, v2);

	return true;
}
//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib4d(index, v0, v1, v2, v3);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib1s(GLuint index, GLshort v0)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib1s(index, v0);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib2s(GLuint index, GLshort v0, GLshort v1)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib2s(index, v0, v1);

	return true;
}

//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib3s(index, v0, v1, v2);

	return true;
}
//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib4s(index, v0, v1, v2, v3);

	return true;
}
//----------------------------------------------------------------------------
bool glShader::setVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3)
{
	if (!useGLSL) return false; // GLSL not available
	if (!_noshader) return true;

	glVertexAttrib4Nub(index, v0, v1, v2, v3);

	return true;
}
//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib1i(GLuint index, GLint v0)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI1iEXT(index, v0);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib2i(GLuint index, GLint v0, GLint v1)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI2iEXT(index, v0, v1);

	return true;
}

//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI3iEXT(index, v0, v1, v2);

	return true;
}
//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI4iEXT(index, v0, v1, v2, v3);

	return true;
}
//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib1ui(GLuint index, GLuint v0)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI1uiEXT(index, v0);

	return true;
}

//-----------------------------------------------------------------------------

bool glShader::setVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI2uiEXT(index, v0, v1);

	return true;
}

//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI3uiEXT(index, v0, v1, v2);

	return true;
}
//-----------------------------------------------------------------------------
bool glShader::setVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
	if (!useGLSL) return false; // GLSL not available
	if (!bGPUShader4) return false;
	if (!_noshader) return true;

	glVertexAttribI4uiEXT(index, v0, v1, v2, v3);

	return true;
}

void glShader::addAttributBlinding(ShaderAttributType type, const std::string& name)
{
	m_attributs_bind[type] = name;
}

void glShader::updateAttributBlinding()
{
	for(MapAttributs::iterator it = m_attributs_bind.begin(); it != m_attributs_bind.end(); it++)
	{
		BindAttribLocation(it->first, it->second.c_str());
	}
}

bool glShader::attributAvailable(ShaderAttributType type)
{
	return m_attributs_bind.find(type) != m_attributs_bind.end();
}

void glShader::addTextureUnit(TextureType type, const std::string& name)
{
	m_textures_bind[type] = name;
}

void glShader::updateTextureUnitsBlinding()
{
	for(MapTexture::iterator it = m_textures_bind.begin(); it != m_textures_bind.end(); it++)
	{
		Logger::Log() << "[INFO] Texture : " << it->second << " : " << (GLint)it->first << "\n";
		setUniform1i(it->second.c_str(), it->first);
	}
}

bool glShader::textureAvailable(TextureType type)
{
	return m_textures_bind.find(type) != m_textures_bind.end();
}

void glShader::addMatrixBinding(MatrixType type, const std::string& name)
{
	m_matrix_bind[type] = name;
}

bool glShader::matrixModeAvailable(MatrixType type)
{
	return m_matrix_bind.find(type) != m_matrix_bind.end();
}

void glShader::OnDraw()
{
  // Nothing to do ....
}

void glShader::UpdateAll()
{
	updateAttributBlinding();
	link();
	// * Warning : Need to active shader
	begin();
	updateTextureUnitsBlinding();
	end();
}

//-----------------------------------------------------------------------------
// ************************************************************************
// Shader Program : Manage Shader Programs (Vertex/Fragment)
// ************************************************************************
glShaderObject::glShaderObject()
{
	InitOpenGLExtensions();
	compiler_log = 0;
	is_compiled = false;
	program_type = 0;
	ShaderObject = 0;
	ShaderSource = 0;
	_memalloc = false;
}

//----------------------------------------------------------------------------- 
glShaderObject::~glShaderObject()
{
	if (compiler_log!=0) free(compiler_log);
	if (ShaderSource!=0)
	{
		if (_memalloc)
			delete[] ShaderSource;  // free ASCII Source
	}

	if (is_compiled)
	{
		glDeleteObjectARB(ShaderObject);
		CHECK_GL_ERROR();
	}
}

//----------------------------------------------------------------------------- 
unsigned long getFileLength(ifstream& file)
{
	if(!file.good()) return 0;

	unsigned long pos=file.tellg();
	file.seekg(0,ios::end);
	unsigned long len = file.tellg();
	file.seekg(ios::beg);

	return len;
}


//----------------------------------------------------------------------------- 
int glShaderObject::load(const char* filename)
{
	ifstream file;
	file.open(filename, ios::in);
	if(!file) return -1;

	unsigned long len = getFileLength(file);

	if (len==0) return -2;   // "Empty File"

	if (ShaderSource!=0)    // there is already a source loaded, free it!
	{
		if (_memalloc)
			delete[] ShaderSource;
	}

	ShaderSource = (GLubyte*) new char[len+1];
	if (ShaderSource == 0) return -3;   // can't reserve memory
	_memalloc = true;

	ShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	unsigned int i=0;
	while (file.good())
	{
		ShaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}

	ShaderSource[i] = 0;  // 0 terminate it.

	file.close();

	return 0;
}

//----------------------------------------------------------------------------- 
void glShaderObject::loadFromMemory(const char* program)
{
	if (ShaderSource!=0)    // there is already a source loaded, free it!
	{
		if (_memalloc)
			delete[] ShaderSource;
	}
	_memalloc = false;
	ShaderSource = (GLubyte*) program;

}


// ----------------------------------------------------------------------------
// Compiler Log: Ausgabe der Compiler Meldungen in String
char* glShaderObject::getCompilerLog(void)
{    
	if (!useGLSL) return aGLSLStrings[0];

	GLint blen = 0;
	GLsizei slen = 0;


	if (ShaderObject==0) return aGLSLStrings[1]; // not a valid program object

	glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH , &blen);
	CHECK_GL_ERROR();

	if (blen > 1)
	{
		if (compiler_log!=0)
		{
			free(compiler_log);
			compiler_log =0;
		}
		if ((compiler_log = (GLcharARB*)malloc(blen)) == NULL)
		{
			printf("ERROR: Could not allocate compiler_log buffer\n");
			return aGLSLStrings[3];
		}

		glGetInfoLogARB(ShaderObject, blen, &slen, compiler_log);
		CHECK_GL_ERROR();
		//cout << "compiler_log: \n", compiler_log);
	}
	if (compiler_log!=0)
		return (char*) compiler_log;
	else
		return aGLSLStrings[6];

	return aGLSLStrings[4];
}

// ----------------------------------------------------------------------------
bool glShaderObject::compile(void)
{
	if (!useGLSL) return false;

	is_compiled = false;

	GLint compiled = 0;

	if (ShaderSource==0) return false;

	GLint	length = (GLint) strlen((const char*)ShaderSource);
	glShaderSourceARB(ShaderObject, 1, (const GLcharARB **)&ShaderSource, &length);
	CHECK_GL_ERROR();

	glCompileShaderARB(ShaderObject);
	CHECK_GL_ERROR();
	glGetObjectParameterivARB(ShaderObject, GL_COMPILE_STATUS, &compiled);
	CHECK_GL_ERROR();

	if (compiled) is_compiled=true;

	return is_compiled;
}

// ----------------------------------------------------------------------------
GLint glShaderObject::getAttribLocation(char* attribName)
{
	return glGetAttribLocationARB(ShaderObject, attribName);
}

// ----------------------------------------------------------------------------
aVertexShader::aVertexShader()
{
	program_type = 1;
	if (useGLSL)
	{
		ShaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		CHECK_GL_ERROR();
	}
}

// ----------------------------------------------------
aVertexShader::~aVertexShader()
{
}
// ----------------------------------------------------

aFragmentShader::aFragmentShader()
{
	program_type = 2;
	if (useGLSL)
	{
		ShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		CHECK_GL_ERROR();
	}
}

// ----------------------------------------------------

aFragmentShader::~aFragmentShader()
{
}


// ----------------------------------------------------

aGeometryShader::aGeometryShader()
{
	program_type = 3;
	if (useGLSL && bGeometryShader)
	{
		ShaderObject = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_EXT);
		CHECK_GL_ERROR();
	}
}

// ----------------------------------------------------

aGeometryShader::~aGeometryShader()
{
}

// ----------------------------------------------------------------------------
// ShaderManager: Easy use of (multiple) Shaders

glShaderManager::glShaderManager()
{
	InitOpenGLExtensions();
	_nInputPrimitiveType = GL_TRIANGLES;
	_nOutputPrimitiveType = GL_TRIANGLE_STRIP;
	_nVerticesOut = 3;

	// slots Connexion
	MatrixManagement::Instance().GetSignalEvent().connect(sigc::mem_fun(*this, &glShaderManager::UpdateMatrix));
}

glShaderManager::~glShaderManager()
{
	// free objects
	vector<glShader*>::iterator  i=_shaderObjectList.begin();
	while (i!=_shaderObjectList.end())
	{
		//glShader* o = *i;
		i=_shaderObjectList.erase(i);
		//delete o;
	}
}

// ----------------------------------------------------------------------------

void glShaderManager::SetInputPrimitiveType(int nInputPrimitiveType)
{
	_nInputPrimitiveType = nInputPrimitiveType;

}

void glShaderManager::SetOutputPrimitiveType(int nOutputPrimitiveType)
{
	_nOutputPrimitiveType = nOutputPrimitiveType;

}

void glShaderManager::SetVerticesOut(int nVerticesOut)
{
	_nVerticesOut = nVerticesOut;
}

// ----------------------------------------------------------------------------
glShader* glShaderManager::loadfromFile(const char* vertexFile, const char* fragmentFile, ShaderType type)
{
	glShader* o;
	if(type == BASIC_SHADER)
		o = new glShader();
	else if(type == GBUFFER_SHADER)
		o = new GBufferShader;
	else
		throw CException("[glShadermanager] Unknow shader type");

	o->UsesGeometryShader(false);

	aVertexShader* tVertexShader = new aVertexShader;
	aFragmentShader* tFragmentShader = new aFragmentShader;

	// load vertex program
	if (vertexFile!=0)
		if (tVertexShader->load(vertexFile) != 0)
		{
			cout << "error: can't load vertex shader!\n";
			delete o;
			delete tVertexShader;
			delete tFragmentShader;

			CHECK_GL_ERROR();

			throw CLoadingFailed(vertexFile,"Impossible de charger le vertex Shader");

			return 0;
		}


	// Load fragment program
	if (fragmentFile!=0)
		if (tFragmentShader->load(fragmentFile) != 0)
		{
			cout << "error: can't load fragment shader!\n";
			delete o;
			delete tVertexShader;
			delete tFragmentShader;

			CHECK_GL_ERROR();

			throw CLoadingFailed(fragmentFile,"Impossible de charger le fragement shader");

			return 0;
		}

	// Compile vertex program
	if (vertexFile!=0)
		if (!tVertexShader->compile())
		{
			cout << "***COMPILER ERROR (Vertex Shader):\n";
			cout << tVertexShader->getCompilerLog() << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;

			CHECK_GL_ERROR();

			throw CLoadingFailed(vertexFile,"Compilation error");

			return 0;
		}
	cout << "***GLSL Compiler Log (Vertex Shader):\n";
	cout << tVertexShader->getCompilerLog() << "\n";

	// Compile fragment program
	if (fragmentFile!=0)
		if (!tFragmentShader->compile())
		{
			cout << "***COMPILER ERROR (Fragment Shader):\n";
			cout << tFragmentShader->getCompilerLog() << endl;

			delete o;
			delete tVertexShader;
			delete tFragmentShader;

			throw CLoadingFailed(fragmentFile,"Compilation error");

			return 0;

		}
	cout << "***GLSL Compiler Log (Fragment Shader):\n";
	cout << tFragmentShader->getCompilerLog() << "\n";

	// Add to object
	if (vertexFile!=0) o->addShader(tVertexShader);
	if (fragmentFile!=0) o->addShader(tFragmentShader);

	// link
	if (!o->link())
	{
		cout << "**LINKER ERROR\n";
		cout << o->getLinkerLog() << endl;
		delete o;
		delete tVertexShader;
		delete tFragmentShader;

		throw CShaderException("Error link.");

		return 0;
	}

	cout << "***GLSL Linker Log:\n";
	cout << o->getLinkerLog() << endl;

	_shaderObjectList.push_back(o);
	o->manageMemory();

	return o;
}


glShader* glShaderManager::loadfromFile(char* vertexFile, char* geometryFile, char* fragmentFile)
{
	glShader* o = new glShader();
	o->UsesGeometryShader(true);
	o->SetInputPrimitiveType(_nInputPrimitiveType);
	o->SetOutputPrimitiveType(_nOutputPrimitiveType);
	o->SetVerticesOut(_nVerticesOut);

	aVertexShader* tVertexShader = new aVertexShader;
	aFragmentShader* tFragmentShader = new aFragmentShader;
	aGeometryShader* tGeometryShader = new aGeometryShader;

	// load vertex program
	if (vertexFile!=0)
		if (tVertexShader->load(vertexFile) != 0)
		{
			cout << "error: can't load vertex shader!\n";
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}

	// Load geometry program
	if (geometryFile!=0)
		if (tGeometryShader->load(geometryFile) != 0)
		{
			cout << "error: can't load geometry shader!\n";
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}

	// Load fragment program
	if (fragmentFile!=0)
		if (tFragmentShader->load(fragmentFile) != 0)
		{
			cout << "error: can't load fragment shader!\n";
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}

	// Compile vertex program
	if (vertexFile!=0)
		if (!tVertexShader->compile())
		{
			cout << "***COMPILER ERROR (Vertex Shader):\n";
			cout << tVertexShader->getCompilerLog() << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}
	cout << "***GLSL Compiler Log (Vertex Shader):\n";
	cout << tVertexShader->getCompilerLog() << "\n";

	// Compile geometry program
	if (geometryFile!=0)
	{

		if (!tGeometryShader->compile())
		{
			cout << "***COMPILER ERROR (Geometry Shader):\n";
			cout << tGeometryShader->getCompilerLog() << endl;

			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;

		}
	}
	cout << "***GLSL Compiler Log (Geometry Shader):\n";
	cout << tGeometryShader->getCompilerLog() << "\n";

	// Compile fragment program
	if (fragmentFile!=0)
		if (!tFragmentShader->compile())
		{
			cout << "***COMPILER ERROR (Fragment Shader):\n";
			cout << tFragmentShader->getCompilerLog() << endl;

			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;

		}
	cout << "***GLSL Compiler Log (Fragment Shader):\n";
	cout << tFragmentShader->getCompilerLog() << "\n";

	// Add to object
	if (vertexFile!=0) o->addShader(tVertexShader);
	if (geometryFile!=0) o->addShader(tGeometryShader);
	if (fragmentFile!=0) o->addShader(tFragmentShader);

	// link
	if (!o->link())
	{
		cout << "**LINKER ERROR\n";
		cout << o->getLinkerLog() << endl;
		delete o;
		delete tVertexShader;
		delete tFragmentShader;
		delete tGeometryShader;
		return 0;
	}
	cout << "***GLSL Linker Log:\n";
	cout << o->getLinkerLog() << endl;

	_shaderObjectList.push_back(o);
	o->manageMemory();

	return o;
	return 0;
}
// ----------------------------------------------------------------------------

glShader* glShaderManager::loadfromMemory(const char* vertexMem, const char* fragmentMem)
{
	glShader* o = new glShader();
	o->UsesGeometryShader(false);

	aVertexShader* tVertexShader = new aVertexShader;
	aFragmentShader* tFragmentShader = new aFragmentShader;

	// get vertex program
	if (vertexMem!=0)
		tVertexShader->loadFromMemory(vertexMem);

	// get fragment program
	if (fragmentMem!=0)
		tFragmentShader->loadFromMemory(fragmentMem);

	// Compile vertex program
	if (vertexMem!=0)
		if (!tVertexShader->compile())
		{
			cout << "***COMPILER ERROR (Vertex Shader):\n";
			cout << tVertexShader->getCompilerLog() << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			return 0;
		}
	cout << "***GLSL Compiler Log (Vertex Shader):\n";
	cout << tVertexShader->getCompilerLog() << "\n";

	// Compile fragment program
	if (fragmentMem!=0)
		if (!tFragmentShader->compile())
		{
			cout << "***COMPILER ERROR (Fragment Shader):\n";
			cout << tFragmentShader->getCompilerLog() << endl;

			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			return 0;
		}
	cout << "***GLSL Compiler Log (Fragment Shader):\n";
	cout << tFragmentShader->getCompilerLog() << "\n";

	// Add to object
	if (vertexMem!=0) o->addShader(tVertexShader);
	if (fragmentMem!=0) o->addShader(tFragmentShader);

	// link
	if (!o->link())
	{
		cout << "**LINKER ERROR\n";
		cout << o->getLinkerLog() << endl;
		delete o;
		delete tVertexShader;
		delete tFragmentShader;
		return 0;
	}
	cout << "***GLSL Linker Log:\n";
	cout << o->getLinkerLog() << endl;

	_shaderObjectList.push_back(o);
	o->manageMemory();

	return o;
}

glShader* glShaderManager::loadfromMemory(const char* vertexMem, const char* geometryMem, const char* fragmentMem)
{
	glShader* o = new glShader();
	o->UsesGeometryShader(true);
	o->SetInputPrimitiveType(_nInputPrimitiveType);
	o->SetOutputPrimitiveType(_nOutputPrimitiveType);
	o->SetVerticesOut(_nVerticesOut);

	aVertexShader* tVertexShader = new aVertexShader;
	aFragmentShader* tFragmentShader = new aFragmentShader;
	aGeometryShader* tGeometryShader = new aGeometryShader;

	// get vertex program
	if (vertexMem!=0)
		tVertexShader->loadFromMemory(vertexMem);

	// get fragment program
	if (fragmentMem!=0)
		tFragmentShader->loadFromMemory(fragmentMem);

	// get fragment program
	if (geometryMem!=0)
		tGeometryShader->loadFromMemory(geometryMem);

	// Compile vertex program
	if (vertexMem!=0)
		if (!tVertexShader->compile())
		{
			cout << "***COMPILER ERROR (Vertex Shader):\n";
			cout << tVertexShader->getCompilerLog() << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}
	cout << "***GLSL Compiler Log (Vertex Shader):\n";
	cout << tVertexShader->getCompilerLog() << "\n";

	// Compile geometry program
	if (geometryMem!=0)
		if (!tGeometryShader->compile())
		{
			cout << "***COMPILER ERROR (Geometry Shader):\n";
			cout << tGeometryShader->getCompilerLog() << endl;
			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}
	cout << "***GLSL Compiler Log (Geometry Shader):\n";
	cout << tVertexShader->getCompilerLog() << "\n";

	// Compile fragment program
	if (fragmentMem!=0)
		if (!tFragmentShader->compile())
		{
			cout << "***COMPILER ERROR (Fragment Shader):\n";
			cout << tFragmentShader->getCompilerLog() << endl;

			delete o;
			delete tVertexShader;
			delete tFragmentShader;
			delete tGeometryShader;
			return 0;
		}
	cout << "***GLSL Compiler Log (Fragment Shader):\n";
	cout << tFragmentShader->getCompilerLog() << "\n";

	// Add to object
	if (vertexMem!=0) o->addShader(tVertexShader);
	if (geometryMem!=0) o->addShader(tGeometryShader);
	if (fragmentMem!=0) o->addShader(tFragmentShader);

	// link
	if (!o->link())
	{
		cout << "**LINKER ERROR\n";
		cout << o->getLinkerLog() << endl;
		delete o;
		delete tVertexShader;
		delete tFragmentShader;
		delete tGeometryShader;
		return 0;
	}
	cout << "***GLSL Linker Log:\n";
	cout << o->getLinkerLog() << endl;

	_shaderObjectList.push_back(o);
	o->manageMemory();

	return o;
}

// ----------------------------------------------------------------------------

bool  glShaderManager::free(glShader* o)
{
	vector<glShader*>::iterator  i=_shaderObjectList.begin();
	while (i!=_shaderObjectList.end())
	{
		if ((*i)==o)
		{
			_shaderObjectList.erase(i);
			delete o;
			return true;
		}
		i++;
	}
	return false;
}

void glShaderManager::Push(glShader* shader)
{
	if(m_shader_stack.size() > m_max_stack)
		throw CException("shader stack is full");
	m_shader_stack.push_back(shader);
	if(m_shader_stack.back()->matrixModeAvailable(MODELVIEW_MATRIX))
		m_shader_stack.back()->setUniformMatrix4fv(MODELVIEW_MATRIX, MatrixManagement::Instance().GetMatrix());
}

void glShaderManager::Pop()
{
	if(m_shader_stack.empty())
		throw CException("shader stack is empty");
	m_shader_stack.pop_back();

	if(!m_shader_stack.empty())
	{
		m_shader_stack.back()->begin();
		if(m_shader_stack.back()->matrixModeAvailable(MODELVIEW_MATRIX))
			m_shader_stack.back()->setUniformMatrix4fv(MODELVIEW_MATRIX, MatrixManagement::Instance().GetMatrix());
	}
}

void glShaderManager::UpdateMatrix(MatrixType mat, const Math::CMatrix4& matrix)
{
	if(!m_shader_stack.empty())
	{
		//Logger::Log() << "[DEBUG] Update Matrix ... \n";
		if(mat == MODELVIEW_MATRIX)
		{
			if(m_shader_stack.back()->matrixModeAvailable(MODELVIEW_MATRIX))
				m_shader_stack.back()->setUniformMatrix4fv(MODELVIEW_MATRIX, matrix);
		}
		else if(mat == PROJECTION_MATRIX)
		{
			if(m_shader_stack.back()->matrixModeAvailable(PROJECTION_MATRIX))
				m_shader_stack.back()->setUniformMatrix4fv(PROJECTION_MATRIX, matrix);
		}
		else
			throw CException("unknow matrix type");
	}

}

TShaderPtr glShaderManager::LoadShader(const std::string& filename)
{
	TShaderPtr Resource = CResourceManager::Instance().Get<glShader>(filename);
	if(Resource == NULL)
	{
		Resource = CMediaManager::Instance().LoadMediaFromFile<glShader>(filename);
		CResourceManager::Instance().Add(filename, Resource);
	}
	return Resource;
}

glShader* glShaderManager::currentShader()
{
	if(!activedShader())
		throw CException("No activated shader...");
	return m_shader_stack.back();
}

bool glShaderManager::activedShader()
{
	return m_shader_stack.size() != 0;
}

//////////////////////////////////////////////////////////////
// G-Buffer Shader
//////////////////////////////////////////////////////////////
GBufferShader::GBufferShader() :
		m_use_texCoord(false),
		m_use_texNormal(false),
		m_use_texDiffuse(false),
		m_use_texSpecular(false),
		m_use_tangants(false),
		m_FBO(NULL)
{
}

GBufferShader::~GBufferShader()
{
}

// The aim of this function is to setup correctly the shader before rendering
void GBufferShader::OnDraw()
{
	glShader::OnDraw();
	// For diffuse textures
	if(m_use_texDiffuse && m_use_texCoord)
	{
		setUniform1i("UseDiffuseTex",1);
	}
	else
	{
		setUniform1i("UseDiffuseTex",0);
	}
	// For normal texture
	if(m_use_texNormal && m_use_tangants && m_use_texCoord)
	{
		setUniform1i("UseBumpMapping",1);
	}
	else
	{
		setUniform1i("UseBumpMapping",0);
	}
	// For specular texture
	if(m_use_texSpecular && m_use_texCoord)
	{
		setUniform1i("UseSpecularTex",1);
	}
	else
	{
		setUniform1i("UseSpecularTex",0);
	}
	// After that reinit all attributs for the next drawing
	m_use_texCoord = false;
	m_use_texNormal = false;
	m_use_texDiffuse = false;
	m_use_texSpecular = false;
	m_use_tangants = false;
}

void GBufferShader::UpdateAll()
{
	glShader::begin();
	// Set good params
	std::vector<FBOTextureBufferParam> buffers;
	// Create all buffers
	FBOTextureBufferParam tex1;
	tex1.Attachment = glGetFragDataLocation(GetProgramObject(), "Diffuse");
	FBOTextureBufferParam tex2;
	tex2.Attachment = glGetFragDataLocation(GetProgramObject(), "Normal");
	FBOTextureBufferParam tex3;
	tex3.Attachment = glGetFragDataLocation(GetProgramObject(), "Specular");
	// Add into a list
	buffers.push_back(tex1);
	buffers.push_back(tex2);
	buffers.push_back(tex3);
	// Create depth buffer param
	FBODepthBufferParam depthParam;
	// Create FBO
	m_FBO = new FBO(Math::TVector2I(800,600), buffers, FBODEPTH_RENDERTARGET, depthParam);
	glShader::end();
	glShader::UpdateAll();
}

bool GBufferShader::attributAvailable(ShaderAttributType type)
{
	//TODO: Rewrite this function
	bool res = glShader::attributAvailable(type);
	if(!res)
		return res;
	// Update attributs
	if(type == TEXCOORD_ATTRIBUT)
		m_use_texCoord = true;
	else if(type == TANGENT_ATTRIBUT)
		m_use_texCoord = true;
	// Return the good value
	return res;
}

void GBufferShader::begin()
{
	m_FBO->Bind();
	glShader::begin();
}

void GBufferShader::end()
{
	m_FBO->UnBind();
	glShader::end();
}

bool GBufferShader::textureAvailable(TextureType type)
{
	//TODO: Rewrite this function
	bool res = glShader::textureAvailable(type);
	if(!res)
		return res;
	// Update attributs
	if(type == DIFFUSE_TEXTURE)
		m_use_texDiffuse = true;
	else if(type == NORMAL_TEXTURE)
		m_use_texNormal = true;
	else if(type == SPECULAR_TEXTURE)
		m_use_texSpecular = false;
	// Return the good value
	return res;
}

