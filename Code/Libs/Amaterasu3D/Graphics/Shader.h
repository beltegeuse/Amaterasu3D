#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <Math/Matrix4.h>
#include <Singleton.h>
#include <Debug/OpenGLDebug.h>
#include <iostream>
//TODO: off screen rendering...

class Shader;
class ShaderUnits;

class Shader
{
private:
	void ShowLinkLog(unsigned int id);
};

class ShaderUnits
{
private:
	// Attributs
	unsigned int m_ID;
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
	unsigned int CompileShader(const std::string& path, ShaderType type);

public:
	// Constructeurs
	ShaderUnits(const std::string& path);
	//Destructor
	~ShaderUnits();
	// Public functions
	void Begin();
	void End();
	// Common shader methods
	inline unsigned int GetAttribLocation(const std::string& name);
};

//***********************
//** Inline functions ***
//***********************
unsigned int ShaderUnits::GetAttribLocation(const std::string& name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetAttribLocation (m_programID,name.c_str()));
	std::cout << "[DEBUG] AttribLocation : " << name << " => " << id << std::endl;
	return id;
}

#endif /* SHADER_H_ */
