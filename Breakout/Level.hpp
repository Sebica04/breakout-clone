#pragma once
#include <string>
#include <vector>
#include "Brick.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"


namespace Level {
	void loadLevel(const std::string& filename, std::vector<Brick>& bricks);

	void resetGame(int& level, Ball& ball, Paddle& paddle, std::vector<Brick>& bricks);
}