#pragma once
#include <SFML/Graphics.hpp>
const unsigned int WINDOW_WIDTH = 960;
const unsigned int WINDOW_HEIGHT = 800;

const float BALL_RADIUS = 12.0f;
const float BALL_SPEED = 3.0f;
const float BALL_ROTATION_FACTOR = 2.0f;
const float GLOW_DURATION_MS = 150.f;

const sf::Vector2f PADDLE_HITBOX_SIZE = { 120.f, 16.f };
const float PADDLE_SPEED = 8.0f;
const float PADDLE_Y_POSITION = WINDOW_HEIGHT - 70.f;

const sf::Vector2f BRICK_SIZE = { 90.f, 30.f };
const float BRICK_PADDING_X = 10.f;
const float BRICK_PADDING_Y = 5.f;

const float LEVEL_START_X = 80.f;
const float LEVEL_START_Y = 80.f;

const float	SCREEN_BORDER = 16.0f;