#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <Math/Matrix4.h>
#include <Singleton.h>
#include <Debug/OpenGLDebug.h>
#include <System/Resource.h>
#include <Graphics/ShaderUnit.h>
#include <iostream>
#include <Enum.h>

/*
 * \brief class which describe one shader
 */


class Shader : public IResource
{
private:
	// Attributs
	ShaderUnit * m_vertex_shader;
	ShaderUnit * m_fragement_shader;
	unsigned int m_programID;
public:
	// Constructor
	Shader(ShaderUnit * VertexShader, ShaderUnit * FragmentShader);
	virtual ~Shader();
	// Public functions
	void Begin();
	void End();
	void SetUniformMatrix4fv(const std::string& name, Math::CMatrix4& matrix);
private:
	void ShowLinkLog(unsigned int id);
	// Common shader methods
	inline unsigned int GetAttribLocation(const std::string& name);
	inline unsigned int GetUniformLocation(const std::string& name);
};

//***********************
//** Inline functions ***
//***********************
unsigned int Shader::GetAttribLocation(const std::string& name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetAttribLocation (m_programID,name.c_str()));
	//std::cout << "[DEBUG] AttribLocation : " << name << " => " << id << std::endl;
	return id;
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetUniformLocation(m_programID,name.c_str()));
	//std::cout << "[DEBUG] UniformLocation : " << name << " => " << id << std::endl;
	return id;
}

#endif /* SHADER_H_ */
