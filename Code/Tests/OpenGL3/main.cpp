#include <SFML/Window.hpp>

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif

int main()
{
	// Create the main window
	sf::Window window(sf::VideoMode(800, 600), "SFML window");

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

		// Fill and active buffer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	    glEnableVertexAttribArray(0);

	    // Draw buffer
	    glDrawArrays(GL_TRIANGLES, 0, 3);

	    // Disable buffer
	    glDisableVertexAttribArray(0);

		// Update the window
		window.Display();
	}

	return 0;
}

