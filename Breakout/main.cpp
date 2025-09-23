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
	play.setPosition({ 960.0f / 2 - 32, 800.0f / 2 - 60 });
	play.setStyle(sf::Text::Bold);

	sf::RectangleShape playButtonBackground;
	playButtonBackground.setSize({ play.getLocalBounds().size.x + 20.f, play.getLocalBounds().size.y + 20.f });
	playButtonBackground.setPosition({play.getPosition().x - 8, play.getPosition().y - 4});
	playButtonBackground.setFillColor(sf::Color(0, 0, 200, 150));
	playButtonBackground.setOutlineColor(sf::Color::Blue);
	playButtonBackground.setOutlineThickness(2.f);


	sf::Text exit(arial);
	exit.setString("Exit");
	exit.setCharacterSize(36);
	exit.setFillColor(sf::Color::Yellow);
	exit.setPosition({ 960.0f / 2 - 28, 800.0f / 2 + 10});
	exit.setStyle(sf::Text::Bold);

	sf::RectangleShape exitButtonBackground;
	exitButtonBackground.setSize({ exit.getLocalBounds().size.x + 20.f, exit.getLocalBounds().size.y + 20.f });
	exitButtonBackground.setPosition({ exit.getPosition().x - 7, exit.getPosition().y - 1 });
	exitButtonBackground.setFillColor(sf::Color(0, 0, 200, 150));
	exitButtonBackground.setOutlineColor(sf::Color::Blue);
	exitButtonBackground.setOutlineThickness(2.f);

	sf::Text scoreText(arial);
	scoreText.setCharacterSize(28);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition({ 30, 10});
	scoreText.setStyle(sf::Text::Bold);

	sf::RectangleShape scoreTextButtonBackground;
	scoreTextButtonBackground.setSize({ 240, 55 });
	scoreTextButtonBackground.setPosition({ 0, 0 });
	scoreTextButtonBackground.setFillColor(sf::Color(0, 0, 200, 150));
	scoreTextButtonBackground.setOutlineColor(sf::Color::Blue);
	scoreTextButtonBackground.setOutlineThickness(2.f);

	sf::Text livesText(arial);
	livesText.setCharacterSize(28);
	livesText.setFillColor(sf::Color::Yellow);
	livesText.setPosition({ 830, 10 });
	livesText.setStyle(sf::Text::Bold);

	sf::RectangleShape livesTextButtonBackground;
	livesTextButtonBackground.setSize({ 160, 55 });
	livesTextButtonBackground.setPosition({ livesText.getPosition().x - 30, 0 });
	livesTextButtonBackground.setFillColor(sf::Color(0, 0, 200, 150));
	livesTextButtonBackground.setOutlineColor(sf::Color::Blue);
	livesTextButtonBackground.setOutlineThickness(2.f);

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
			myWindow->draw(playButtonBackground);
			myWindow->draw(play);
			myWindow->draw(exitButtonBackground);
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
			myWindow->draw(scoreTextButtonBackground);
			myWindow->draw(scoreText);
			myWindow->draw(livesTextButtonBackground);
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

