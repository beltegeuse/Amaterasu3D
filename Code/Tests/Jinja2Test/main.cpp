
// STD Includes
#include <iostream>
#include <map>
#include <set>

// Ama3D includes
#include <Debug/Exceptions.h>
#include <System/PythonInterpreter.h>
#include <System/Python/Jinja2Template.h>

void Test(const std::string& fullpath)
{
	ama3D::Jinja2Template jinja2(fullpath);
	jinja2.AddArgument("toto", "add :)");
	jinja2.Generate();

	// Print the result
	std::cout << "[INFO] Result : " << std::endl;
	std::cout << jinja2.GetCode() << std::endl;
}

int main()
{
	Test("../Donnees/Templates/test.txt");
	Test("../Donnees/Templates/test2.txt");
	Test("../Donnees/Templates/extends/test_extends_child.txt");
	return 0;
}



