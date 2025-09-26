#include "Level.hpp"
#include "fstream"
#include <iostream>
#include <sstream>


void Level::loadLevel(const std::string& filename, std::vector<Brick>& bricks) {
	bricks.clear();
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to load level file" << std::endl;
		return;
	}
	std::string line;
	float currentY = 100.f;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string token;
		float currentX = 80.f;

		
		while (ss >> token) {
			if (token != "0") {
				char typeChar = token[0];
				char colorChar = token[1];

				Brick::Type brickType;
				sf::Color brickColor;

				switch (typeChar) {
				case '1': 
					brickType = Brick::Type::Normal;
					break;
				case '2':
					brickType = Brick::Type::Sturdy;
					break;
				case '3':
					brickType = Brick::Type::Steel;
					break;
				}

				switch (colorChar) {
				case 'r':
					brickColor = sf::Color::Red;
					break;
				case 'g':
					brickColor = sf::Color::Green;
					break;
				case 'b':
					brickColor = sf::Color::Blue;
					break;
				case 'y':
					brickColor = sf::Color::Yellow;
					break;
				case 'w':
					brickColor = sf::Color::White;
					break;
				}

				bricks.emplace_back(currentX, currentY, brickType, brickColor);
			}
			currentX += 90.f + 10;
		}
		currentY += 30.f + 5;
	}
}

void Level::resetGame(int& level, int& score, int& lives, Ball& ball, Paddle& paddle, std::vector<Brick>& bricks) {

	if (level == 1) {
		score = 0;
		lives = 3;
	}

	
	paddle.setPosition({ 960.0f / 2, 800.0f - 70.0f });
	ball.hold(paddle);

	
	std::string levelFile = "assets/levels/level" + std::to_string(level) + ".txt";

	loadLevel(levelFile, bricks);
}