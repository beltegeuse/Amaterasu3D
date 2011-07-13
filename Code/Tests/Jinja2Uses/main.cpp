#include <Python.h>
#include <iostream>
#include <map>
#include <Debug/Exceptions.h>
const std::string CODE_JINJA = "\n"
		"filename = os.path.split(args['fullpath'])[-1]\n"
		"dirpath = os.path.join(os.path.split(args['fullpath'])[:-1])[0]\n"
		"print '[SCRIPT] Filename : '+filename\n"
		"print '[SCRIPT] Dir path : '+dirpath\n"
		"loader = jinja2.FileSystemLoader(['../Donnees/Shaders/HelpersCode', dirpath])\n"
		"environment = jinja2.Environment(loader=loader)\n"
		"tpl = environment.get_template(filename)\n"
		"res = tpl.render()\n";

class PythonArgs
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
	PythonArgs() {m_Dict = PyDict_New();}
	virtual ~PythonArgs() { Release(); }

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

private:
	void AddArgument(const std::string& name, PyObject* obj)
	{
		if(m_Args.find(name) != m_Args.end())
			throw ama3D::CException("Key "+name+" already in the set!");
		m_Args[name] = obj;
		PyDict_SetItemString(m_Dict, name.c_str(), obj);
	}

	void Release()
	{
		Py_XDECREF(m_Dict);
		for(std::map<std::string, PyObject*>::iterator it = m_Args.begin();
			it != m_Args.end(); ++it)
		{
			Py_XDECREF(it->second);
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
	void Execute(const std::string& code, PythonArgs& args)
	{
		std::cout << "[INFO] Execute : " << std::endl;
		std::cout << code << std::endl;

		PyDict_SetItemString(m_MainDict, "args", args.GetObject());
		PyRun_SimpleString(code.c_str());

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
	python.Import("os");
	PythonArgs args;
	args.AddArgument("fullpath", "../Donnees/Templates/test.txt");
	python.Execute(CODE_JINJA, args);
	return 0;
}

