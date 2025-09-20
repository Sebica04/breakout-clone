#include "Brick.hpp"
#include <iostream>

sf::Texture Brick::s_texture;
bool Brick::s_textureLoaded = false;

Brick::Brick(float startX, float startY, const sf::Color& color) {
	
	if (!s_textureLoaded) {
		if (s_texture.loadFromFile("assets/images/brick.png")) {
			s_textureLoaded = true;
		}
		else {
			std::cerr << "Failed to load brick texture!" << std::endl;
		}
	}
	setPosition({ startX, startY });

	m_color = color;
	m_shape.setSize({90.f, 30.f});
	m_shape.setTexture(&s_texture);
	m_shape.setFillColor(m_color);
	m_shape.setOrigin(m_shape.getGeometricCenter());

	

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


void Brick::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!m_isDestroyed) {
		states.transform *= getTransform();
		target.draw(m_shape, states);
	}
}