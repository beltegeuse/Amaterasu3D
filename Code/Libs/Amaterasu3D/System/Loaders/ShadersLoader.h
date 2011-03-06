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

class ShadersLoader : public ILoader<glShader>
{
public:
	ShadersLoader();
	virtual ~ShadersLoader();
	virtual glShader* LoadFromFile(const std::string& Filename);
};

#endif /* SHADERLOADER_H_ */
