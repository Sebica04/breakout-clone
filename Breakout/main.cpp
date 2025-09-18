#include <iostream>
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include <fstream>
#include "TileMap.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
using json = nlohmann::json;

int main() {
	unsigned int width = 960;
	unsigned int height = 800;

	sf::RenderWindow* myWindow = new sf::RenderWindow(sf::VideoMode({ width, height }), "Breakout");

	myWindow->setFramerateLimit(144);

	TileMap map;

	if (!map.load("assets/images/tilemap.json", "assets/images/tileset.png")) {
		std::cerr << "Failed to load the map!" << std::endl;
		return -1;
	}


	Ball ball(960.0f / 2, 800.0f / 2);

	Paddle paddle(960.0f / 2, 800.0f - 70.0f);

	while (myWindow->isOpen()) {

		while (const std::optional event = myWindow->pollEvent()) {

			if (event->is<sf::Event::Closed>())
				myWindow->close();
		}

		ball.update(paddle);
		paddle.update();


		myWindow->clear();
		myWindow->draw(map);
		myWindow->draw(ball);
		myWindow->draw(paddle);
		myWindow->display();
	}

	delete myWindow;
	return 0;
}

