#include "SFML/Window.hpp"

int main(int argc, char *argv[])
{
	// Request a 24-bits depth buffer when creating the window
	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 24;

	// Create the main window
	sf::Window window(sf::VideoMode(640, 480), "Zero Test Game: Empty OpenGL Window", sf::Style::Default, contextSettings);

	// Make it the active window for OpenGL calls
	window.setActive();

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event{};
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// Escape key: exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		// Finally, display the rendered frame on screen
		window.display();
	}
}
