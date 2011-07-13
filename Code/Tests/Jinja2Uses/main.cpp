#include <iostream>
#include <map>
#include <set>
#include <Debug/Exceptions.h>
#include <System/PythonInterpreter.h>

const std::string CODE_JINJA = "\n"
		"filename = os.path.split(args['fullpath'])[-1]\n"
		"dirpath = os.path.join(os.path.split(args['fullpath'])[:-1])[0]\n"
		"print '[SCRIPT] Filename : '+filename\n"
		"print '[SCRIPT] Dir path : '+dirpath\n"
		"loader = jinja2.FileSystemLoader(['.', '../Donnees/Shaders/HelpersCode', dirpath])\n"
		"environment = jinja2.Environment(loader=loader)\n"
		"tpl = environment.get_template(filename)\n"
		"res = tpl.render(args['template'])\n";

void Test(ama3D::PythonInterpreter& python, const std::string& fullpath)
{
	// Call
	ama3D::PythonMap templat;
	templat.AddArgument("toto", "add :)");

	ama3D::PythonMap args;
	args.AddArgument("fullpath", fullpath);
	args.AddArgument("template", templat);
	ama3D::PythonResult res;
	res.AddVariable("res");
	python.Execute(CODE_JINJA, args, res);

	// Print the result
	std::cout << "[INFO] Result : " << std::endl;
	std::cout << res["res"] << std::endl;
}

int main()
{
	// Initialisation de l'interpreteur
	ama3D::PythonInterpreter python;
	python.Import("jinja2");
	python.Import("os");

	Test(python, "../Donnees/Templates/test.txt");
	Test(python, "../Donnees/Templates/test2.txt");

	return 0;
}

