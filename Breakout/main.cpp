#include <iostream>
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include <fstream>
#include <sstream>  
#include "TileMap.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
using json = nlohmann::json;



enum class GameState {
	MainMenu,
	GameRunning,
	GameOver
};

void loadLevel(const std::string& filename, std::vector<Brick>& bricks) {

	bricks.clear();
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: Failed to open level file: " << filename << std::endl;
		return;
	}
	std::string line;
	float currentY = 80.f;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string token;
		float currentX = 80.f;

		while (ss >> token)
		{
			if (token != "0")
			{
				char typeChar = token[0];
				char colorChar = token[1];

				Brick::Type brickType = Brick::Type::Normal;
				sf::Color brickColor = sf::Color::White;

				switch (typeChar) {
				case '1': brickType = Brick::Type::Normal; break;
				case '2': brickType = Brick::Type::Sturdy; break;
				case '3': brickType = Brick::Type::Steel; break;
				}

				switch (colorChar) {
				case 'r': brickColor = sf::Color::Red; break;
				case 'g': brickColor = sf::Color::Green; break;
				case 'b': brickColor = sf::Color::Blue; break;
				case 'y': brickColor = sf::Color::Yellow; break;
				case 'w': brickColor = sf::Color::White; break;
				}

				bricks.emplace_back(currentX, currentY, brickType, brickColor);
			}
			currentX += 90.f + 10.f;
		}
		currentY += 30.f + 5.f; 
	}
}

void resetGame(int& level, int& score, int& lives, Ball& ball, Paddle& paddle, std::vector<Brick>& bricks) {

	ball.setPosition({ 960.0f / 2, 800.0f / 2 });
	paddle.setPosition({ 960.0f / 2, 800.0f - 70.0f });

	std::string levelFile = "assets/levels/level" + std::to_string(level) + ".txt";

	loadLevel(levelFile, bricks);
}

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
						resetGame(currentLevel, score, lives, ball, paddle, bricks);
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
				resetGame(currentLevel, score, lives, ball, paddle, bricks);
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

