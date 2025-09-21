#include <iostream>
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include <fstream>
#include "TileMap.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
using json = nlohmann::json;



enum class GameState {
	MainMenu,
	GameRunning,
	GameOver
};

int main() {



	//~~~~~~~~~~~~~~~~~~~~Creating the window~~~~~~~~~~~~~~~~~~//

	unsigned int width = 960;
	unsigned int height = 800;

	sf::RenderWindow* myWindow = new sf::RenderWindow(sf::VideoMode({ width, height }), "Breakout");

	myWindow->setFramerateLimit(144);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//






	GameState currentState = GameState::MainMenu;

	sf::Font arial;
	if (!arial.openFromFile("../dependencies/arial.ttf")) { 
		std::cerr << "Failed to load font!" << std::endl;
		return -1;
	}

	TileMap map;

	if (!map.load("assets/images/tilemap.json", "assets/images/tileset.png")) {
		std::cerr << "Failed to load the map!" << std::endl;
		return -1;
	}

	sf::Text play(arial);
	play.setString("Play");
	play.setCharacterSize(36);
	play.setFillColor(sf::Color::Yellow);
	play.setPosition({ 960.0f / 2 - 30, 800.0f / 2 - 36 });
	play.setStyle(sf::Text::Bold);
	//~~~~~~~Initializing the game objects~~~~~~~~~~~~~~~~~//
			//~~~~Tilemap for backgorund, ball, paddle, bricks~~~~~~~~//
	Ball ball(960.0f / 2, 800.0f / 2);

	Paddle paddle(960.0f / 2, 800.0f - 70.0f);

	std::vector<Brick> bricks;
	std::vector<sf::Color> colors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow };

	const int bricksPerRow = 9;
	const int numRows = 4;
	for (int i = 0; i < bricksPerRow; ++i) {
		for (int j = 0; j < numRows; ++j) {
			float x = i * (90.f + 10.f) + 80.f;
			float y = j * (30.f + 10.f) + 50.f;
			bricks.emplace_back(x, y, colors[j % colors.size()]);
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//




	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Game loop~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	while (myWindow->isOpen()) {

		switch (currentState) {

		case GameState::MainMenu: {
			while (const std::optional event = myWindow->pollEvent()) {

				if (event->is<sf::Event::Closed>())
					myWindow->close();
				
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i mouseTPos = sf::Mouse::getPosition(*myWindow);
					sf::Vector2f mousePos = myWindow->mapPixelToCoords(mouseTPos);

					if (play.getGlobalBounds().contains(mousePos)) {
						currentState = GameState::GameRunning;
					}
				}
			}
			myWindow->clear(sf::Color::Black);
			myWindow->draw(map);
			myWindow->draw(play);
			myWindow->display();
			break;
		}
		case GameState::GameRunning:{
			while (const std::optional event = myWindow->pollEvent()) {

				if (event->is<sf::Event::Closed>())
					myWindow->close();
			}

			ball.update(paddle, bricks);
			paddle.update();

			//~~~~~~~~~~erasing the destroyed briks~~~~~~~~~~~~//
			int i = 0;
			while (i < bricks.size())
			{
				if (bricks[i].isDestroyed())
				{
					bricks.erase(bricks.begin() + i);
				}
				else
				{
					i++;
				}
			}

			myWindow->clear();
			myWindow->draw(map);
			myWindow->draw(ball);
			myWindow->draw(paddle);

			for (const auto& brick : bricks) {
				myWindow->draw(brick);
			}
			myWindow->display();
			break;
		}
		case GameState::GameOver:
		{
			// We'll implement this later
			break;
		}
		}

		
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


	delete myWindow;
	return 0;
}

