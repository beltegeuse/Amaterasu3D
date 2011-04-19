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
		Event event;
		while(win.PoolEvent(event))
		{
			std::cout << "Event !" << std::endl;
			if(event.Type == Event::Closed)
			{
				std::cout << "Close ... " << std::endl;
				win.Close();
			}
			else if(event.Type == Event::KeyPressed)
			{
				if(event.Key.Code == Key::Escape)
				{
					std::cout << "Close ... " << std::endl;
					win.Close();
				}
			}
		}

		//std::cout << "Display !" << std::endl;

		win.Display();
	}

	std::cout << "... End" << std::endl;
	return 0;
}
