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
#include <map>

// Amaterasu3D Includes
#include <Debug/Exceptions.h>

namespace ama3D
{
////////////////////////////////////////
/// Exception class For ShaderCompiler
////////////////////////////////////////
struct CShaderCompilerException: public CException
{
	CShaderCompilerException(const std::string& message, int line = -1);
};

//////////////////////////////////////////
/// Structure to configure the compiler
//////////////////////////////////////////
////////////// Main Config structure

class ShaderCompilerConfig
{
public:
	enum DefineType
	{
		DEFINE_BOOL,
		DEFINE_STRING
	};

	static DefineType GetDefineType(const std::string& typeName)
	{
		if(typeName == "Bool")
			return DEFINE_BOOL;
		else if(typeName == "String")
			return DEFINE_STRING;
		else
			throw CException("Unknow Define type : " + typeName);
	}

	struct DefineEntry
	{
		DefineType type;
		std::string value;
	};

	static DefineEntry GetDefineEntry(const std::string& typeName, const std::string& value)
	{
		DefineEntry entry;
		entry.type = GetDefineType(typeName);
		entry.value = value;
		return entry;
	}

	typedef std::map<std::string, DefineEntry> DefineMap;
private:
	/*
	 * Attributes
	 */
	DefineMap m_Defines;
public:
	ShaderCompilerConfig()
	{
	}
	virtual ~ShaderCompilerConfig()
	{
	}

	void AddDefine(const std::string& name, const DefineEntry& value)
	{
		m_Defines[name] = value;
	}

	const DefineMap& GetDefines() const
	{
		return m_Defines;
	}
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
	ShaderCompilerConfig m_Config;
public:
	/*
	 * Constructors and Destructors
	 */
	ShaderCompiler(const std::string& code, const ShaderCompilerConfig& config =
			ShaderCompilerConfig());
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
	void ResolveDefinesRules();
	const std::string LoadFile(const std::string& path); //XXX Doublon code with ShaderUnits
};
}

#endif /* SHADERCOMPILER_H_ */
