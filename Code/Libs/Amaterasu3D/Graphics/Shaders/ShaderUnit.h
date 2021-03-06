//==========================================================
// Amaterasu3D - perceptual 3D engine
//
// Copyright (C) 2010-2011 Adrien Gruson
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

#ifndef SHADERUNIT_H_
#define SHADERUNIT_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <System/Resource.h>
#include <Enum.h>
#include <Graphics/Shaders/Compiler/ShaderCompiler.h>

namespace ama3D
{
class ShaderUnit
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
	ShaderUnit(const std::string& path, const ShaderUnitType& type,
			const ShaderCompilerConfig& config);
	//Destructor
	~ShaderUnit();

	unsigned int GetID();
};
}

#endif /* SHADERUNIT_H_ */
