#include <Python.h>
#include <iostream>
#include <map>

const std::string CODE_TEMPLATE = ""
		"__kernel void test({{test2}}) {"
		"	return ;"
		"}";

const std::string CODE_JINJA = "\n"
		"tpl = jinja2.Template(code)\n"
		"res = tpl.render(args)";

class PythonObject
{
private:
	PyObject* m_Object;
public:
	PythonObject(PyObject* obj) :
		m_Object(obj)
	{}
	virtual ~PythonObject() { Py_XDECREF(m_Object); }
	inline PyObject* GetPythonObject() { return m_Object; }
};

class PythonStringObject : public PythonObject
{
public:
	PythonStringObject(const std::string& s = "") :
		PythonObject(PyString_FromString(s.c_str()))
	{}

	virtual std::string Repr() { return std::string(PyString_AsString(GetPythonObject())); }
};

class PythonArgs
{
private:
	std::map< std::string, PythonObject*> m_Args;

public:
	PythonArgs() {}
	virtual ~PythonArgs() {}

	void AddArgument(const std::string& name, PythonObject* obj)
	{
		//TODO: Check collision
		m_Args[name] = obj;
	}

	void Release()
	{
		for(std::map<std::string, PythonObject*>::iterator it = m_Args.begin();
			it != m_Args.end(); ++it)
		{
			delete it->second;
		}
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
	PythonInterpreter() : m_MainModule(0), m_MainDict(0)
	{
		std::cout << "[INFO] Create python interpreter ..." << std::endl;
		Py_Initialize();

		m_MainModule = PyImport_ImportModule("__main__");
		m_MainDict = PyModule_GetDict(m_MainModule);

		Import("sys");
		PyRun_SimpleString("print '  * Python Version : '+str(sys.version)");

	}

	virtual ~PythonInterpreter()
	{
		std::cout << "[INFO] Destruct python interpreter." << std::endl;
		Py_Finalize();
	}

	void Import(const std::string& module)
	{
		PyObject *pLoadingModule = PyImport_ImportModule(module.c_str());
		PyDict_SetItemString(m_MainDict, module.c_str(), pLoadingModule);
	}

	/*
	 * Public methods
	 */
	void Execute(const std::string& code)
	{
		std::cout << "[INFO] Execute : " << std::endl;
		std::cout << code << std::endl;

		PyObject* pCode = PyString_FromString(CODE_TEMPLATE.c_str());
		PyDict_SetItemString(m_MainDict, "code", pCode);

		PyObject* pArgs = PyDict_New();
		PyDict_SetItemString(pArgs, "test2", PyString_FromString("toto"));
		PyDict_SetItemString(m_MainDict, "args", pArgs);

		PyRun_SimpleString(code.c_str());
		Py_XDECREF(pCode);

		PyObject* pRes = PyMapping_GetItemString(m_MainDict, "res");
		std::cout << "[INFO] Result : " << std::endl;
		std::cout << PyString_AsString(pRes) << std::endl;
		Py_XDECREF(pRes);
	}
};

int main()
{
	PythonInterpreter python;
	python.Import("jinja2");
	python.Execute(CODE_JINJA);
	return 0;
}

