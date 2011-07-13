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

#include "PythonInterpreter.h"

#include <Logger/Logger.h>

namespace ama3D
{

/*=================================================
 * PythonMap
 *=================================================
 */
void PythonMap::Release()
{
	Py_DECREF(m_Dict);
	for(std::map<std::string, PyObject*>::iterator it = m_Args.begin();
		it != m_Args.end(); ++it)
	{
		Py_DECREF(it->second);
	}
}

void PythonResult::Update(PyObject* dict)
{
	m_Res.clear();
	for(std::set<std::string>::iterator it = m_Variables.begin(); it != m_Variables.end(); ++it)
	{
		PyObject* pRes = PyMapping_GetItemString(dict, const_cast < char* >(it->c_str()));
		m_Res[*it] = std::string(PyString_AsString(pRes));
		Py_DECREF(pRes);
	}
}
/*=================================================
 * PythonInterpreter
 *=================================================
 */
PythonInterpreter::PythonInterpreter() : m_MainModule(0), m_MainDict(0)
{
	std::cout << "[INFO] Create python interpreter ..." << std::endl;
	Py_Initialize();

	m_MainModule = PyImport_ImportModule("__main__");
	m_MainDict = PyModule_GetDict(m_MainModule);

	Import("sys");
	PyRun_SimpleString("print '  * Python Version : '+str(sys.version)");
}

PythonInterpreter::~PythonInterpreter()
{
	Logger::Log() << "[INFO] Destruct python interpreter.\n";
	Py_Finalize();
}

void PythonInterpreter::Import(const std::string& module)
{
	PyObject *pLoadingModule = PyImport_ImportModule(module.c_str());
	PyDict_SetItemString(m_MainDict, module.c_str(), pLoadingModule);
}

void PythonInterpreter::Execute(const std::string& code, PythonMap& args, PythonResult& res)
{
		Logger::Log() << "[DEBUG] Execute python script =======\n";
		Logger::Log() << code;
		Logger::Log() << "[DEBUG] =============================\n";

		//TODO: Gerer le nettoye du dictionnaire
		//TODO: Handle exception
		PyDict_SetItemString(m_MainDict, "args", args.GetObject());
		PyRun_SimpleString(code.c_str());
		res.Update(m_MainDict);
}

} /* namespace ama3D */
