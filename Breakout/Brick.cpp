#include "Brick.hpp"
#include <iostream>

sf::Texture Brick::s_texture;
bool Brick::s_textureLoaded = false;

Brick::Brick(float startX, float startY, Type type, const sf::Color& color) {
	
	if (!s_textureLoaded) {
		if (s_texture.loadFromFile("assets/images/brick_spritesheet.png")) {
			s_textureLoaded = true;
		}
		else {
			std::cerr << "Failed to load brick texture!" << std::endl;
		}
	}
	setPosition({ startX, startY });

	m_shape.setSize({90.f, 30.f});
	m_shape.setTexture(&s_texture);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getGeometricCenter());

	m_type = type;

	switch (m_type)
	{
	case Type::Normal:
		m_health = 1;
		break;
	case Type::Sturdy:
		m_health = 2;
		break;
	case Type::Steel:
		m_health = -1;
		break;
	}
	updateTexture();
	m_isDestroyed = false;
}

sf::FloatRect Brick::getGlobalBounds() const {
	return getTransform().transformRect(m_shape.getGlobalBounds());
}

void Brick::destroy() {
	m_isDestroyed = true;
}

bool Brick::isDestroyed() const {
	return m_isDestroyed;
}

void Brick::updateTexture()
{
	sf::IntRect textureRect;
	switch (m_type)
	{
	case Type::Normal:
		textureRect = { {0, 0}, {90, 30} };
		break;
	case Type::Sturdy:
		if (m_health == 2)
			textureRect = { {90, 0}, {90, 30} };
		else
			textureRect = { {180, 0}, {90, 30} };
		break;
	case Type::Steel:
		textureRect = { {270, 0}, {90, 30} };
		break;
	}
	m_shape.setTextureRect(textureRect);
}

void Brick::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!m_isDestroyed) {
		states.transform *= getTransform();
		target.draw(m_shape, states);
	}
}

void Brick::takeDamage() {
	m_health -= 1;
	updateTexture();
}

int Brick::health() {
	return m_health;
}