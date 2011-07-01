//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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
#include <Math/Vector2.h>
#include <System/Resource.h>
#include <Logger/Logger.h>
#include <Graphics/Shaders/ShaderUnit.h>
#include <Graphics/Shaders/Compiler/ShaderCompiler.h>
#include <Graphics/FBO.h>
#include <Graphics/Color.h>
#include <Utilities/SmartPtr.h>
#include <System/SettingsManager.h>
#include <Graphics/MatrixManagement.h>

/*
 * \brief class which describe one shader
 */
namespace ama3D
{
class FBO;

struct CShaderException: public CException
{
	CShaderException(const std::string& message);
};

class Shader: public IResource
{
private:
	// Attributes
	ShaderUnit * m_VertexShader;
	ShaderUnit * m_FragementShader;
	ShaderUnit * m_GeometryShader;
	GLuint m_ProgramID;
	FBO * m_FBO;
	bool m_IsLink;
	// Cache system
//	typedef std::map<std::string, GLint> CachedIDMap;
//	CachedIDMap m_CachedID;
	// All binding attributs
	typedef std::map<ShaderAttributType, std::string> MapAttributs;
	typedef std::map<int, std::string> MapTexture;
	typedef std::map<MatrixType, std::string> MapMatrix;
	typedef std::map<MaterialType, std::string> MapMaterials;
	MapAttributs m_attributs_bind;
	MapTexture m_textures_bind;
	MapMatrix m_matrix_bind;
	MapMaterials m_material_bind;
public:
	/*
	 * Constructors & Destructors
	 */
	Shader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader,
			ShaderUnit * GeometryShader = 0);
	virtual ~Shader();

	/*
	 * Public Methods
	 */
	// ***** Common Shader methods
	void Begin();
	void End();
	GLuint GetProgramObject();
	// To setup the geometry shader
	void SetGeometryShaderParameters(GLenum inputMode, GLenum outputMode,
			int output = -1); ///< -1 mean max
	int MaxOutputVertices();
	///////////////////////
	// Uniforms Setters
	///////////////////////
	// **** 1 Dimension Settings
	void SetUniform1f(const GLcharARB* varname, GLfloat v0);
	void SetUniform1fv(const GLcharARB* varname, int nbValues, GLfloat *tab);
	void SetUniform1i(const GLcharARB* varname, GLint v0);
	void SetUniform1iv(const GLcharARB* varname, int nbValues, GLint *tab);
	void SetUniform1ui(const GLcharARB* varname, GLuint v0);
	void SetUniform1uiv(const GLcharARB* varname, int nbValues, GLuint *tab);
	// **** 2 Dimension Settings
	void SetUniformVector(const GLcharARB* varname,
			const Math::TVector2F& vector);
	// **** 3 Dimension Settings
	void SetUniformVector(const GLcharARB* varname,
			const Math::TVector3F& vector);
	void SetUniformVectorArray(const GLcharARB* varname, int nbValues,
			Math::TVector3F *tab);
	// **** 4 Dimension Settings
	void SetUniformVector(const GLcharARB* varname,
			const Math::TVector4F& vector);
	void SetUniformColor(const GLcharARB* varname, Color& color);
	// **** 3x3 Matrice Settings
	// **** 4x4 Matrice Settings
	void SetUniformMatrix4fv(const GLchar* name, const Math::CMatrix4& matrix);
	void SetUniformMatrix4fv(MatrixType type, const Math::CMatrix4& matrix);

	// **** Attributes locations
	void BindAttribLocation(GLint index, const GLchar* name);
	// FBO Management
	void SetFBO(FBO* fbo, bool deletePrevious = true);
	FBO* GetFBO();
	bool IsFBOAvaliable() const;
	//////////////////////////////
	// Shader Attributes Bindings
	//////////////////////////////
	// * Attributs
	void AddAttributBinding(ShaderAttributType type, const std::string& name);
	void UpdateAttributBinding();
	virtual bool IsAttributAvailable(ShaderAttributType type);
	// * Textures
	void AddTextureUnit(int typeID, const std::string& name);
	void UpdateTextureUnitsBinding();
	virtual bool IsTextureAvailable(TextureType type);
	// * matrix
	void AddMatrixBinding(MatrixType type, const std::string& name);
	virtual bool IsMatrixModeAvailable(MatrixType type);
	void UpdateMatrix(MatrixType mat);
	void UpdateMatrixAll();
	// * materials
	void AddMaterialBinding(MaterialType type, const std::string& name);
	bool IsMaterialAvailable(MaterialType type);
	void SetMaterialValue(MaterialType type, Color& color);
	// To know before drawing
	virtual void OnDraw();
	virtual void UpdateAll();
	// To Link the Program
	void Link();

private:
	void ShowLinkLog(unsigned int id);
	// Common shader methods
	inline int GetAttribLocation(const GLchar* name);
	inline GLint GetUniformLocation(const GLchar* name);
};

typedef CSmartPtr<Shader, CResourceCOM> TShaderPtr;

///////////////////////////////////
/// Shader Manager
//////////////////////////////////

class CShaderManager: public CSingleton<CShaderManager>
{
MAKE_SINGLETON(CShaderManager)
	// Implements Singleton
	CShaderManager();
	virtual ~CShaderManager();

	friend class ShadersLoader;
	// Only the MediaManager can call this methods
	// Regular GLSL (Vertex+Fragment Shader)
	Shader* loadfromFile(const char* vertexFile, const char* fragmentFile,
			ShaderType type, const ShaderCompilerConfig& config); //!< load vertex/fragment shader from file. If you specify 0 for one of the shaders, the fixed function pipeline is used for that part. \param vertexFile Vertex Shader File. \param fragmentFile Fragment Shader File.
	Shader* loadfromFile(const char* vertexFile, const char* fragmentFile,
			const char* geometryFile, ShaderType type,
			const ShaderCompilerConfig& config);
	Shader* CreateShader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader,
			ShaderUnit * GeometryShader = 0, ShaderType type = BASIC_SHADER);
public:

	// Only this methods for load shader
	TShaderPtr LoadShader(const std::string& filename);

	void SetInputPrimitiveType(int nInputPrimitiveType); //!< Set the input primitive type for the geometry shader \param nInputPrimitiveType Input Primitive Type, for example GL_TRIANGLES
	void SetOutputPrimitiveType(int nOutputPrimitiveType); //!< Set the output primitive type for the geometry shader \param nOutputPrimitiveType Output Primitive Type, for example GL_TRIANGLE_STRIP
	void SetVerticesOut(int nVerticesOut); //!< Set the maximal number of vertices the geometry shader can output \param nVerticesOut Maximal number of output vertices. It is possible to output less vertices!

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
	/*
	 * Attributes
	 */
	std::vector<Shader*> m_shader_stack;
	int m_max_stack;

	std::vector<Shader*> _shaderObjectList;
	int _nInputPrimitiveType;
	int _nOutputPrimitiveType;
	int _nVerticesOut;
};

//***********************
//** Helper functions
//***********************
#include <System/SettingsManager.h>
#include <Graphics/MatrixManagement.h>

inline void ShaderHelperUniformImagePlane(Shader* shader)
{
	shader->SetUniform1f("NearClipping",
			CSettingsManager::Instance().GetNearClipping());
	Math::TVector2F UnprojectInfo;
	Math::CMatrix4 ProjectionMatrix = CMatrixManager::Instance().GetMatrix(
			PROJECTION_MATRIX);
	UnprojectInfo.x = 1.0f / ProjectionMatrix.a11;
	UnprojectInfo.y = -1.0f / ProjectionMatrix.a22;
	shader->SetUniformVector("UnprojectInfo", UnprojectInfo);
	shader->SetUniformMatrix4fv("InverseViewMatrix",
			CMatrixManager::Instance().GetMatrix(VIEW_MATRIX).Inverse());
}

inline void ShaderHelperUniformPositionFromView(Shader* shader)
{
	shader->SetUniform1f("FarClipping",
			CSettingsManager::Instance().GetFarClipping());
	ShaderHelperUniformImagePlane(shader);
}

inline void ShaderHelperUniformPosition(Shader* shader,
		const Math::CMatrix4& projectionMatrix,
		const Math::CMatrix4& viewMatrix, float nearValue, float farValue)
{
	shader->SetUniform1f("FarClipping", farValue);
	shader->SetUniform1f("NearClipping", nearValue);
	Math::TVector2F UnprojectInfo;
	UnprojectInfo.x = 1.0f / projectionMatrix.a11;
	UnprojectInfo.y = -1.0f / projectionMatrix.a22;
	shader->SetUniformVector("UnprojectInfo", UnprojectInfo);
	shader->SetUniformMatrix4fv("InverseViewMatrix", viewMatrix.Inverse());
}

//***********************
//** Inline functions ***
//***********************
int Shader::GetAttribLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	GLint id = GLCheck(glGetAttribLocation (m_ProgramID,name));
	return id;
}

GLint Shader::GetUniformLocation(const GLchar* name)
{
//	CachedIDMap::iterator it = m_CachedID.find(std::string(name));
//	if(it != m_CachedID.end())
//		return it->second;
	//FIXME: Faire du caching pour accelerer les appels
	GLint loc = glGetUniformLocation(m_ProgramID, name);
	if (loc == -1)
	{
		Logger::Log() << "Error: can't find uniform variable \"" << name
				<< "\"\n";
	}
//	else
//	{
//		m_CachedID[std::string(name)] = loc;
//	}
	return loc;
}
}

#endif /* SHADER_H_ */
