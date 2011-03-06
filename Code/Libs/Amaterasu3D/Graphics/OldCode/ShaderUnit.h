/*
 * ShaderUnit.h
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#ifndef SHADERUNIT_H_
#define SHADERUNIT_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <System/Resource.h>
#include <Enum.h>

class ShaderUnit : public IResource
{
private:
	// Attributs
	unsigned int m_ID;
private:
	// Private function
	/**
	 * Load Shader File...
	 */
	const std::string LoadFile(const std::string& path);
	void ShowCompilerLog(unsigned int id);
public:
	// Constructeurs
	ShaderUnit(const std::string& path, const ShaderUnitType& type);
	//Destructor
	~ShaderUnit();

	unsigned int GetID();
};

#endif /* SHADERUNIT_H_ */
