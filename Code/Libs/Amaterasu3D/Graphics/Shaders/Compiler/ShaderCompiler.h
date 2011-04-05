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

#ifndef SHADERCOMPILER_H_
#define SHADERCOMPILER_H_

// STL Includes
#include <string>
#include <list>

// Amaterasu3D Includes
#include <Debug/Exceptions.h>

////////////////////////////////////////
/// Exception class For ShaderCompiler
////////////////////////////////////////
struct CShaderCompilerException : public CException
{
	CShaderCompilerException(const std::string& message, int line = -1);
};

////////////////////////////////////////
/// Shader Compiler class
////////////////////////////////////////
class ShaderCompiler
{
private:
	/*
	 * Attributes
	 */
	std::list<std::string> m_LinesCode;
public:
	/*
	 * Constructors and Destructors
	 */
	ShaderCompiler(const std::string& code);
	virtual ~ShaderCompiler();

	/*
	 * Public Methods
	 */
	void Compile();
	void AnalyseCompilerLog(const std::string& log);

	const std::string GetCode() const;

private:
	/*
	 * Private methods
	 */
	void ResolveIncludeRules();
	const std::string LoadFile(const std::string& path); //XXX Doublon code with ShaderUnits
};

#endif /* SHADERCOMPILER_H_ */
