/*
 * ShaderLoader.h
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#ifndef SHADERLOADER_H_
#define SHADERLOADER_H_

#include <System/Loaders/Loader.h>
#include <Graphics/Shader.h>
#include <Enum.h>

class ShadersLoader : public ILoader<ShaderUnit>
{
private:
	ShaderUnitType m_Type;
public:
	ShadersLoader(const ShaderUnitType& type);
	virtual ~ShadersLoader();
	virtual ShaderUnit* LoadFromFile(const std::string& Filename);
};

#endif /* SHADERLOADER_H_ */
