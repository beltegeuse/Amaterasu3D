#include <Python.h>
#include <iostream>
#include <map>
#include <set>
#include <Debug/Exceptions.h>
const std::string CODE_JINJA = "\n"
		"filename = os.path.split(args['fullpath'])[-1]\n"
		"dirpath = os.path.join(os.path.split(args['fullpath'])[:-1])[0]\n"
		"print '[SCRIPT] Filename : '+filename\n"
		"print '[SCRIPT] Dir path : '+dirpath\n"
		"loader = jinja2.FileSystemLoader(['.', '../Donnees/Shaders/HelpersCode', dirpath])\n"
		"environment = jinja2.Environment(loader=loader)\n"
		"tpl = environment.get_template(filename)\n"
		"res = tpl.render(args['template'])\n";

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

	void Update(PyObject* dict)
	{
		m_Res.clear();
		for(std::set<std::string>::iterator it = m_Variables.begin(); it != m_Variables.end(); ++it)
		{
			PyObject* pRes = PyMapping_GetItemString(dict, const_cast < char* >(it->c_str()));
			m_Res[*it] = std::string(PyString_AsString(pRes));
			Py_XDECREF(pRes);
		}
	}

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
	void Execute(const std::string& code, PythonMap& args, PythonResult& res)
	{
		std::cout << "[INFO] Execute : " << std::endl;
		std::cout << code << std::endl;

		//TODO: Gerer le nettoye du dictionnaire
		PyDict_SetItemString(m_MainDict, "args", args.GetObject());
		PyRun_SimpleString(code.c_str());
		res.Update(m_MainDict);
	}
};

void Test(PythonInterpreter& python, const std::string& fullpath)
{
	// Call
	PythonMap templat;
	templat.AddArgument("toto", "add :)");

	PythonMap args;
	args.AddArgument("fullpath", fullpath);
	args.AddArgument("template", templat);
	PythonResult res;
	res.AddVariable("res");
	python.Execute(CODE_JINJA, args, res);

	// Print the result
	std::cout << "[INFO] Result : " << std::endl;
	std::cout << res["res"] << std::endl;
}

int main()
{
	// Initialisation de l'interpreteur
	PythonInterpreter python;
	python.Import("jinja2");
	python.Import("os");

	Test(python, "../Donnees/Templates/test.txt");
	Test(python, "../Donnees/Templates/test2.txt");

	return 0;
}

