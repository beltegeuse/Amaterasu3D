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
	void SetUniformMatrix4fv(const GLchar* name, Math::CMatrix4& matrix);
private:
	void ShowLinkLog(unsigned int id);
	// Common shader methods
	inline int GetAttribLocation(const GLchar* name);
	inline int GetUniformLocation(const GLchar* name);
};

//***********************
//** Inline functions ***
//***********************
int Shader::GetAttribLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	unsigned int id = GLCheck(glGetAttribLocation (m_programID,name));
	//std::cout << "[DEBUG] AttribLocation : " << name << " => " << id << std::endl;
	return id;
}

int Shader::GetUniformLocation(const GLchar* name)
{
	//FIXME: Faire du caching pour accelerer les appels
	GLint loc = GLCheck(glGetUniformLocation(m_programID,name));
	std::cout << "[DEBUG] UniformLocation : " << name << " => " << loc << " ( program " << m_programID << " )" <<  std::endl;
	return loc;
}

#endif /* SHADER_H_ */
