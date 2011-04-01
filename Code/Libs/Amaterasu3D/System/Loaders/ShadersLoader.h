//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2004-2005 Adrien Gruson
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
#ifndef SHADERLOADER_H_
#define SHADERLOADER_H_

#include <System/Loaders/Loader.h>
#include <Graphics/Shaders/Shader.h>
#include <Enum.h>
#include <tinyxml.h>

class ShadersLoader : public ILoader<Shader>
{
public:
	ShadersLoader();
	virtual ~ShadersLoader();
	virtual Shader* LoadFromFile(const std::string& Filename);
private:
	void LoadShaderAttributs(Shader* shader, TiXmlElement *root);
	void LoadShaderTextures(Shader* shader, TiXmlElement *root);
	void LoadShaderMatrix(Shader* shader, TiXmlElement *root);
	void LoadShaderFBO(Shader* shader, TiXmlElement *root);
	void LoadMaterials(Shader* shader, TiXmlElement *root);
};

#endif /* SHADERLOADER_H_ */
