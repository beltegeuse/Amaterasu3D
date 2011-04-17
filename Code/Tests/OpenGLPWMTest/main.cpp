#include <iostream>

#include <Window.h>
#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::cout << "Lancement ..." << std::endl;
	Window win;
	WindowMode mode(800,600);
	std::cout << "Creation ..." << std::endl;
	win.Create(mode,"OpenGLPWM");

	std::cout << "Affichage ..." << std::endl;
	while(win.IsOpened())
	{
		win.Display();
	}

	std::cout << "... End" << std::endl;
	return 0;
}
