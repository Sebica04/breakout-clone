#include <iostream>
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include <fstream>
#include "TileMap.hpp"

using json = nlohmann::json;

int main() {
	unsigned int width = 960;
	unsigned int height = 800;

	sf::RenderWindow* myWindow = new sf::RenderWindow(sf::VideoMode({ width, height }), "Breakout");

	TileMap map;

	if (!map.load("assets/images/tilemap.json", "assets/images/tileset.png")) {
		std::cerr << "Failed to load the map!" << std::endl;
		return -1;
	}

	while (myWindow->isOpen()) {

		while (const std::optional event = myWindow->pollEvent()) {

			if (event->is<sf::Event::Closed>())
				myWindow->close();
		}
		myWindow->clear();
		myWindow->draw(map);
		myWindow->display();
	}

	delete myWindow;
	return 0;
}

