#include "Paddle.hpp"
#include <iostream>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~constructor~~~~~~~~~~~~~~~~~~~~~~~~~~//


Paddle::Paddle(float startX, float startY) : m_sprite(m_texture) { 
	
	setPosition({ startX, startY });

	if (!m_texture.loadFromFile("assets/images/paddle.png")) {
		std::cerr << "Failed to load the paddle texture!" << std::endl;
	}

	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin({ 130.0f / 2.f, 50.0f / 2.f });

	m_hitbox.setSize(PADDLE_HITBOX_SIZE);
	m_hitbox.setFillColor(sf::Color::Transparent);
	m_hitbox.setOrigin(m_hitbox.getGeometricCenter());

	m_currentState = 0;
	m_isHit = false;
	updateSprite();
	m_speed = 4.0f;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~helper functions~~~~~~~~~~~~~~~~~~~~~~~~//

sf::Vector2f Paddle::getPosition() const { return sf::Transformable::getPosition(); }

sf::Vector2f Paddle::getSize() const { return m_hitbox.getSize(); }

sf::FloatRect Paddle::getGlobalBounds() const { return getTransform().transformRect(m_hitbox.getGlobalBounds()); }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//



void Paddle::update() {

	bool isMovingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
	bool isMovingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

	int baseState = 0;

	if (isMovingLeft && !isMovingRight) {
		baseState = 1;
	}

	else if (isMovingRight && !isMovingLeft) {
		baseState = 2;
	}

	if (m_isHit && m_hitTimer.getElapsedTime().asMilliseconds() > GLOW_DURATION_MS) {
		m_isHit = false;
	}

	//telling if the paddle is moving and if it was hit
	m_currentState = baseState + (m_isHit ? 3 : 0);

	if (baseState == 1 && getPosition().x > 81) move({ -m_speed, 0.f });
	if (baseState == 2 && getPosition().x < 879) move({ m_speed, 0.f });

	updateSprite();

}

void Paddle::onBallHit() {
	m_isHit = true;
	m_hitTimer.restart();
}

void Paddle::updateSprite() {

	//getting the right rectangle texture for the paddle
	int stateIndex = m_currentState % 3;
	int stateRow = m_currentState / 3;

	int rectLeft = stateIndex * 130;
	int rectTop = stateRow * 50;

	m_sprite.setTextureRect(sf::IntRect({ rectLeft, rectTop }, { 130, 50 }));
}

void Paddle::draw(sf::RenderTarget& target, sf::RenderStates states)const {
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}