#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

#include <SFML/Window.hpp>
#include <iostream>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::cout << "[INFO] Start the application ..." << std::endl;

	sf::ContextSettings settings;
	settings.MajorVersion = 3;
	settings.MinorVersion = 1;

	std::cout << "[INFO] Try to open OpenGL window." << std::endl;
	// Create the main window
	sf::Window window(sf::VideoMode(800, 600), "SFML window",sf::Style::Default,  settings);

	std::cout << "[INFO] Try to initialise GLEW module for OpenGL 3.1 context..." << std::endl;
	// initialise GLEW module
	GLenum initialisationGLEW = glewInit();
	// Si l'initialisation a échouée :
	if(initialisationGLEW != GLEW_OK)
	{
		std::cout << "[ERROR] Impossible d'initialiser GLEW...." << std::endl;
		return 1;
	}


	// Vertices
	float vertices[] = {-0.5, -0.5,   0.0, 0.5,   0.5, -0.5};

	std::cout << "[INFO] Go to the rendering loop ...." << std::endl;
	// Start the game loop
	while (window.IsOpened())
	{
		// Process events
		sf::Event event;
		while (window.GetEvent(event))
		{
			// Close window : exit
			if (event.Type == sf::Event::Closed)
				window.Close();
		}

		//Clear
		glClear(GL_COLOR_BUFFER_BIT);

		// On remplie puis on active le tableau Vertex Attrib 0
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(0);


		// On affiche le triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// On désactive le tableau Vertex Attrib puisque l'on n'en a plus besoin
		glDisableVertexAttribArray(0);

		// Update the window
		window.Display();
	}

	return 0;
}

