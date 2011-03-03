#include <iostream>

#include <Graphics/Window.h>
#include <windows.h>

class ConcreteWindow : public Window
{
public:
	ConcreteWindow() :
		Window("Amaterasu3DTestApp")
	{
	}

	virtual ~ConcreteWindow()
	{
	}

	virtual void OnDraw()
	{
		Window::OnDraw();
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::cout << "[INFO] Begin ..." << std::endl;
	ConcreteWindow window;
	window.Run();
	std::cout << "[INFO] ... end." << std::endl;
	return 0;
}
