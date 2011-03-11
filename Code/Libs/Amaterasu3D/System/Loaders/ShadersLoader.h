/*
 * ShaderLoader.h
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#ifndef SHADERLOADER_H_
#define SHADERLOADER_H_

#include <System/Loaders/Loader.h>
#include <Graphics/GLSLShader.h>
#include <Enum.h>
#include <tinyxml.h>

class ShadersLoader : public ILoader<glShader>
{
public:
	ShadersLoader();
	virtual ~ShadersLoader();
	virtual glShader* LoadFromFile(const std::string& Filename);
private:
	void LoadShaderAttributs(glShader* shader, TiXmlElement *root);
	void LoadShaderTextures(glShader* shader, TiXmlElement *root);
	void LoadShaderMatrix(glShader* shader, TiXmlElement *root);
	void LoadShaderFBO(glShader* shader, TiXmlElement *root);
};

#endif /* SHADERLOADER_H_ */
