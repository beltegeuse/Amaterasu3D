#include <SFML/Window.hpp>

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

int main()
{
	sf::ContextSettings settings;
	settings.MajorVersion = 3;
	settings.MinorVersion = 1;
	// Create the main window
	sf::Window window(sf::VideoMode(800, 600), "SFML window",sf::Style::Default,  settings);

	// Vertices
	float vertices[] = {-0.5, -0.5,   0.0, 0.5,   0.5, -0.5};

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

//		// Fill and active buffer
//		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
//	    glEnableVertexAttribArray(0);
//
//	    // Draw buffer
//	    glDrawArrays(GL_TRIANGLES, 0, 3);
//
//	    // Disable buffer
//	    glDisableVertexAttribArray(0);

		// Update the window
		window.Display();
	}

	return 0;
}

