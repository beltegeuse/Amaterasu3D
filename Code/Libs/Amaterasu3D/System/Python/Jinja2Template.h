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

#ifndef JINJA2TEMPLATE_H_
#define JINJA2TEMPLATE_H_

#include <System/PythonInterpreter.h>

namespace ama3D
{

class Jinja2Template : public PythonMap
{
private:
	/*
	 * Attributes
	 */
	const static std::string CODE_JINJA; // < Python code
	PythonInterpreter m_Python; //< to interpret the python code
	PythonMap m_Args; //< common var for PythonInterpreter::Execute
	std::string m_Result;
	std::string m_File;
public:
	/*
	 * Constructors & Destructors
	 */
	Jinja2Template(const std::string& file);
	virtual ~Jinja2Template();

	/*
	 * Public methods
	 */
	void Generate();
	const std::string& GetCode() const;
};

} /* namespace ama3D */
#endif /* JINJA2TEMPLATE_H_ */
