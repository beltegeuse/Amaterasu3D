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

#include "Jinja2Template.h"

namespace ama3D
{

const std::string Jinja2Template::CODE_JINJA = "\n"
		"filename = os.path.split(args['fullpath'])[-1]\n"
		"dirpath = os.path.join(os.path.split(args['fullpath'])[:-1])[0]\n"
		"print '[SCRIPT] Filename : '+filename\n"
		"print '[SCRIPT] Dir path : '+dirpath\n"
		"loader = jinja2.FileSystemLoader(['.', '../Donnees/Shaders/HelpersCode', dirpath])\n"
		"environment = jinja2.Environment(loader=loader)\n"
		"tpl = environment.get_template(filename)\n"
		"res = tpl.render(args['template'])\n";

Jinja2Template::Jinja2Template(const std::string& file) :
		m_File(file)
{
	// Setup Args
	m_Args.AddArgument("template", *this);
	m_Args.AddArgument("fullpath", m_File);

	// Import modules
	m_Python.Import("os");
	m_Python.Import("jinja2");
}

Jinja2Template::~Jinja2Template()
{
}

void Jinja2Template::Generate()
{
	PythonResult res;
	res.AddVariable("res");
	m_Python.Execute(CODE_JINJA, m_Args, res);
	m_Result = res["res"];
}

const std::string& Jinja2Template::GetResult() const
{
	return m_Result;
}

} /* namespace ama3D */
