#include <iostream>
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include <fstream>
#include <sstream>  
#include "TileMap.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Level.hpp"
#include "HighScore.hpp"


using json = nlohmann::json;



enum class GameState {
	MainMenu,
	GameRunning,
	GameOver
};



int main() {

	bool checkedForHighScore = false;

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


	std::vector<ScoreEntry> highScores;
	HighScore::loadScores(highScores);

	sf::Text gameOverTitle(arial);
	gameOverTitle.setCharacterSize(28);
	gameOverTitle.setString("GameOver");

	sf::Text yourScoreText(arial);
	yourScoreText.setCharacterSize(40);
	yourScoreText.setString("Your Score: ");

	std::vector<sf::Text> highScoreTexts;

	for (int i = 0; i < 5; ++i) {
		sf::Text text(arial);
		text.setCharacterSize(32);
		text.setFillColor(sf::Color::White);
		text.setPosition({ 960.f / 2.f - 100.f, 300.f + (i * 40.f) });
		highScoreTexts.push_back(text);
	}

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


			if (lives == 0) {
				checkedForHighScore = false; 
				currentState = GameState::GameOver;
			}

			while (const std::optional event = myWindow->pollEvent()) {

				if (event->is<sf::Event::Closed>())
					myWindow->close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
					bricks.clear();
				}
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
				currentState = GameState::GameOver;
			myWindow->display();
			break;
		}
		case GameState::GameOver:
		{
			while (const std::optional event = myWindow->pollEvent()) {

				if (event->is<sf::Event::Closed>())
					myWindow->close();
			}

			if (!checkedForHighScore) {
				// 1. Update the list and see if we made a new high score.
				bool newHighScoreMade = HighScore::updateAndCheck(score, highScores);

				// 2. If we did, save the new list to the file.
				if (newHighScoreMade) {
					HighScore::saveScores(highScores);
				}

				checkedForHighScore = true;

				// 3. Update the display text (this is now always correct)
				yourScoreText.setString("Your Score: " + std::to_string(score));
				// Clear old text before setting new text
				for (auto& text : highScoreTexts) {
					text.setString("");
				}
				for (int i = 0; i < highScores.size(); ++i) {
					highScoreTexts[i].setString(std::to_string(i + 1) + ".   " + std::to_string(highScores[i].score));
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
				currentState = GameState::MainMenu;
			}
			myWindow->clear();
			myWindow->draw(gameOverTitle);
			myWindow->draw(yourScoreText);
			for (const auto& text : highScoreTexts) {
				myWindow->draw(text);
			}
			myWindow->display();
			break;
		}
		}

		
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


	delete myWindow;
	return 0;
}

