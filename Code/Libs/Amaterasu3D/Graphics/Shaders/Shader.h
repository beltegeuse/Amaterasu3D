//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc.,
// 59 Temple Place - Suite 330,
// Boston, MA  02111-1307, USA.
//
// E-mail : adrien.gruson@gmail.com
//==========================================================
#ifndef SHADER_H_
#define SHADER_H_

// GL includes
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// STL includes
#include <string>
#include <map>
#include <vector>

// Amaterasu3D includes
#include <Singleton.h>
#include <Enum.h>
#include <Debug/OpenGLDebug.h>
#include <Math/Matrix4.h>
#include <System/Resource.h>
#include <Logger/Logger.h>
#include <Graphics/Shaders/ShaderUnit.h>
#include <Graphics/FBO.h>
#include <Graphics/Color.h>
#include <Utilities/SmartPtr.h>

/*
 * \brief class which describe one shader
 */

class FBO;

struct CShaderException : public CException
{
	CShaderException(const std::string& message);
};

class Shader : public IResource
{
private:
	// Attributes
	ShaderUnit * m_VertexShader;
	ShaderUnit * m_FragementShader;
	GLuint m_ProgramID;
	FBO * m_FBO;
	bool m_IsLink;
	// All binding attributs
	typedef std::map<ShaderAttributType,std::string> MapAttributs;
	typedef std::map<int,std::string> MapTexture;
	typedef std::map<MatrixType,std::string> MapMatrix;
	typedef std::map<MaterialType,std::string> MapMaterials;
	MapAttributs m_attributs_bind;
	MapTexture m_textures_bind;
	MapMatrix m_matrix_bind;
	MapMaterials m_material_bind;
public:
	// Constructor
	Shader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader);
	virtual ~Shader();
	// Public functions
	void Begin();
	void End();
	// Uniforms Setters
	// **** 1 Dimension Settings
	void setUniform1f(const GLcharARB* varname, GLfloat v0);
	void setUniform1i(const GLcharARB* varname, GLint v0);
	void setUniform1ui(const GLcharARB* varname, GLuint v0);
	// **** 2 Dimension Settings
	// **** 3 Dimension Settings
	// **** 4 Dimension Settings
	void SetUniformColor(const GLcharARB* varname, Color& color);
	// **** 3x3 Matrice Settings
	// **** 4x4 Matrice Settings
	void SetUniformMatrix4fv(const GLchar* name, const Math::CMatrix4& matrix);
	void setUniformMatrix4fv(MatrixType type, const Math::CMatrix4& matrix);
	// **** Attributes locations
	void BindAttribLocation(GLint index, const GLchar* name);
	// FBO Management
	void SetFBO(FBO* fbo, bool deletePrevious = true);
	FBO* GetFBO();
	bool IsFBOAvaliable() const;

	GLuint GetProgramObject();
	//////////////////////////////
	// Shader Attributes Bindings
	//////////////////////////////
	// * Attributs
	void addAttributBlinding(ShaderAttributType type, const std::string& name);
	void updateAttributBlinding();
	virtual bool attributAvailable(ShaderAttributType type);
	// * Textures
	void addTextureUnit(int typeID, const std::string& name);
	void updateTextureUnitsBlinding();
	virtual bool textureAvailable(TextureType type);
	// * matrix
	void addMatrixBinding(MatrixType type, const std::string& name);
	virtual bool matrixModeAvailable(MatrixType type);
	void UpdateMatrix(MatrixType mat);
	void UpdateMatrixAll();
	// * materials
	void addMaterialBinding(MaterialType type, const std::string& name);
	bool materialAvailable(MaterialType type);
	void setMaterialValue(MaterialType type, Color& color);
	// To know before drawing
	virtual void OnDraw();
	virtual void UpdateAll();


private:
	// To Link the Program
	void Link();
	void ShowLinkLog(unsigned int id);
	// Common shader methods
	inline int GetAttribLocation(const GLchar* name);
	inline GLint GetUniformLocation(const GLchar* name);
};

typedef CSmartPtr<Shader, CResourceCOM> TShaderPtr;

///////////////////////////////////
/// Shader Manager
//////////////////////////////////

class CShaderManager : public CSingleton<CShaderManager>
{
	MAKE_SINGLETON(CShaderManager)
	// Implements Singleton
	CShaderManager();
	virtual ~CShaderManager();

	friend class ShadersLoader;
	// Only the MediaManager can call this methods
	// Regular GLSL (Vertex+Fragment Shader)
	Shader* loadfromFile(const char* vertexFile, const char* fragmentFile, ShaderType type);    //!< load vertex/fragment shader from file. If you specify 0 for one of the shaders, the fixed function pipeline is used for that part. \param vertexFile Vertex Shader File. \param fragmentFile Fragment Shader File.

public:

	// Only this methods for load shader
	TShaderPtr LoadShader(const std::string& filename);

	void      SetInputPrimitiveType(int nInputPrimitiveType);    //!< Set the input primitive type for the geometry shader \param nInputPrimitiveType Input Primitive Type, for example GL_TRIANGLES
	void      SetOutputPrimitiveType(int nOutputPrimitiveType);  //!< Set the output primitive type for the geometry shader \param nOutputPrimitiveType Output Primitive Type, for example GL_TRIANGLE_STRIP
	void      SetVerticesOut(int nVerticesOut);                  //!< Set the maximal number of vertices the geometry shader can output \param nVerticesOut Maximal number of output vertices. It is possible to output less vertices!

	//FIXME: Look how to connect to the ressource manager
	bool free(Shader* o); //!< Remove the shader and free the memory occupied by this shader.

	//* Multiple shader management
	void Push(Shader* shader);
	void Pop();
	// Callback to update matrix
	void UpdateMatrix(MatrixType mat);
	Shader* currentShader();
	bool activedShader();
private:
	std::vector<Shader*>  m_shader_stack;
	int m_max_stack;

	std::vector<Shader*>  _shaderObjectList;
	int                     _nInputPrimitiveType;
	int                     _nOutputPrimitiveType;
	int                     _nVerticesOut;
};

//***********************
//** Inline functions ***
//***********************
int Shader::GetAttribLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetAttribLocation (m_ProgramID,name));
	//std::cout << "[DEBUG] AttribLocation : " << name << " => " << id << std::endl;
	return id;
}

GLint Shader::GetUniformLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	GLint loc = glGetUniformLocation(m_ProgramID,name);
	if(loc == -1)
	{
		Logger::Log() << "Error: can't find uniform variable \"" << name << "\"\n";
	}
	//std::cout << "[DEBUG] UniformLocation : " << name << " => " << loc << " ( program " << m_ProgramID << " )" <<  std::endl;
	return loc;
}

#endif /* SHADER_H_ */
