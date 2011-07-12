#include <Python.h>
#include <iostream>

const std::string CODE_TEMPLATE = ""
		"__kernel void test({{test2}}) {"
		"	return ;"
		"}";

const std::string CODE_JINJA = "\n"
		"from jinja2 import Template\n"
		"tpl = Template(code)\n"
		"res = tpl.render(args)";

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
	}

	virtual ~PythonInterpreter()
	{
		std::cout << "[INFO] Destruct python interpreter." << std::endl;
		Py_Finalize();
	}

	void Import(const std::string& module)
	{
		//PyObject*
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
	python.Execute(CODE_JINJA);
	return 0;
}

