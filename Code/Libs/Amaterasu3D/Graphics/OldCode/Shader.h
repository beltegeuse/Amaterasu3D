#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <Math/Matrix4.h>
#include <Debug/OpenGLDebug.h>
#include <iostream>
//TODO: Faire le rendu off screen
class Shader
{
private:
	// Attributs
	unsigned int m_vertexID;
	unsigned int m_fragmentID;
	unsigned int m_programID ;
	bool m_is_build;
	// Type of shader
	enum ShaderType
	{
		VERTEX,
		FRAGMENT
	};
private:
	// Private function
	/**
	 * Load Shader File...
	 */
	const std::string LoadFile(const std::string& path);
	void ShowCompilerLog(unsigned int id);
	void ShowLinkLog(unsigned int id);
	unsigned int CompileShader(const std::string& path, ShaderType type);

public:
	// Constructeurs
	Shader();
	Shader(const std::string& pathVertex, const std::string& pathFragment);
	//Destructor
	~Shader();
	// Public functions
	void Begin();
	void End();
	// Common shader methods
	inline unsigned int GetAttribLocation(const std::string& name);
	void SetUniformMatrix4fv(const std::string& name, Math::CMatrix4& matrix);
};

//***********************
//** Inline functions ***
//***********************
unsigned int Shader::GetAttribLocation(const std::string& name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetAttribLocation (m_programID,name.c_str()));
	std::cout << "[DEBUG] AttribLocation : " << name << " => " << id << std::endl;
	return id;
}

#endif /* SHADER_H_ */
