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

#ifndef PYTHONINTERPRETER_H_
#define PYTHONINTERPRETER_H_

// Ama3D includes
#include <Debug/Exceptions.h>

// Python main include
#include <Python.h>

// STD includes
#include <string>
#include <map>
#include <set>

//
#ifndef WIN32
#define PythonRelease(x) Py_DECREF(x)
#else
#define PythonRelease(x)
#endif

namespace ama3D
{

class PythonMap
{
private:
	/*
	 * Attributes
	 */
	std::map< std::string, PyObject*> m_Args;
	bool m_Build;
	PyObject* m_Dict;
public:
	/*
	 * Constructors & Destructors
	 */
	PythonMap() {m_Dict = PyDict_New();}
	virtual ~PythonMap() { Release(); }

	PyObject* GetObject() { return m_Dict; }
	/*
	 * Add methodes
	 */
	void AddArgument(const std::string& name, const std::string& v)
	{
		AddArgument(name,PyString_FromString(v.c_str()));
	}

	void AddArgument(const std::string& name, long v)
	{
		AddArgument(name, PyInt_FromLong(v));
	}

	void AddArgument(const std::string& name, double v)
	{
		AddArgument(name, PyFloat_FromDouble(v));
	}

	void AddArgument(const std::string& name, PythonMap& map)
	{
		AddArgument(name, map.GetObject());
	}

private:
	void AddArgument(const std::string& name, PyObject* obj)
	{
		if(m_Args.find(name) != m_Args.end())
			throw CException("Key "+name+" already in the set!");
		m_Args[name] = obj;
		PyDict_SetItemString(m_Dict, name.c_str(), obj);
	}

	void Release();
};

class PythonResult
{
private:
	/*
	 * Attributes
	 */
	std::set<std::string> m_Variables;
	std::map<std::string,std::string> m_Res;
public:
	PythonResult() {}
	virtual ~PythonResult() {}

	void AddVariable(const std::string& v)
	{
		m_Variables.insert(v);
	}

	void ClearVariables()
	{
		m_Variables.clear();
	}

	void Update(PyObject* dict);

	const std::string& operator[](const std::string& key)
	{
		//FIXME Add exceptions
		return m_Res[key];
	}
};

class PythonInterpreter
{
private:
	/*
	 * Attributes
	 */
	PyObject * m_MainModule;
	PyObject * m_MainDict;
public:
	/*
	 * Constructors & Destructors
	 */
	PythonInterpreter();
	virtual ~PythonInterpreter();

	/*
	 * Public methods
	 */
	void Import(const std::string& module);
	void Execute(const std::string& code, PythonMap& args, PythonResult& res);
};

} /* namespace ama3D */
#endif /* PYTHONINTERPRETER_H_ */
