#include <iostream>
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include <fstream>
#include <sstream>  
#include "TileMap.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Level.hpp"
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
	play.setPosition({ 960.0f / 2 - 32, 800.0f / 2 - 50 });
	play.setStyle(sf::Text::Bold);

	sf::Text exit(arial);
	exit.setString("Exit");
	exit.setCharacterSize(36);
	exit.setFillColor(sf::Color::Yellow);
	exit.setPosition({ 960.0f / 2 - 30, 800.0f / 2 });
	exit.setStyle(sf::Text::Bold);

	sf::Text scoreText(arial);
	scoreText.setCharacterSize(28);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition({ 30, 10});
	scoreText.setStyle(sf::Text::Bold);

	sf::Text livesText(arial);
	livesText.setCharacterSize(28);
	livesText.setFillColor(sf::Color::Yellow);
	livesText.setPosition({ 830, 10 });
	livesText.setStyle(sf::Text::Bold);

	//~~~~~~~Initializing the game objects~~~~~~~~~~~~~~~~~//
			//~~~~Tilemap for backgorund, ball, paddle, bricks~~~~~~~~//
	int score = 0;
	int lives = 3;

	Ball ball(960.0f / 2, 800.0f / 2);

	Paddle paddle(960.0f / 2, 800.0f - 70.0f);

	std::vector<Brick> bricks;

	int currentLevel = 1;

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
						currentLevel = 1;
						Level::resetGame(currentLevel, ball, paddle, bricks);
						currentState = GameState::GameRunning;
					}
					else if (exit.getGlobalBounds().contains(mousePos)) {
						myWindow->close();
					}
				}
			}
			myWindow->clear(sf::Color::Black);
			myWindow->draw(map);
			myWindow->draw(play);
			myWindow->draw(exit);
			myWindow->display();
			break;
		}
		case GameState::GameRunning:{
			while (const std::optional event = myWindow->pollEvent()) {

				if (event->is<sf::Event::Closed>())
					myWindow->close();
			}

			ball.update(paddle, bricks, score, lives);
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
			bool levelCleared = true;

			for (const auto& brick : bricks)
			{
				
				if (brick.getType() != Brick::Type::Steel)
				{
					levelCleared = false;
					break;
				}
			}

			if (levelCleared) {
				currentLevel += 1;
				Level::resetGame(currentLevel, ball, paddle, bricks);
			}

			scoreText.setString("Score: " + std::to_string(score));
			livesText.setString("Lives: " + std::to_string(lives));
			myWindow->clear();
			myWindow->draw(map);
			myWindow->draw(ball);
			myWindow->draw(paddle);
			myWindow->draw(scoreText);
			myWindow->draw(livesText);

			for (const auto& brick : bricks) {
				myWindow->draw(brick);
			}
			if (lives == 0)
				currentState = GameState::MainMenu;
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

