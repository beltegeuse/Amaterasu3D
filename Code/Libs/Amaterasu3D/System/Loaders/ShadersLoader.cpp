/*
 * ShaderLoader.cpp
 *
 *  Created on: Mar 4, 2011
 *      Author: Adrien
 */

#include "ShadersLoader.h"

ShadersLoader::ShadersLoader(const ShaderUnitType& type) :
m_Type(type)
{
}

ShadersLoader::~ShadersLoader()
{
}

ShaderUnit* ShadersLoader::LoadFromFile(const std::string& Filename)
{
	return new ShaderUnit(Filename, m_Type);
}
