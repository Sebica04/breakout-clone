#include "Brick.hpp"

Brick::Brick(float startX, float startY) {
	setPosition({ startX, startY });

	m_shape.setSize({60.f, 20.f});
	m_shape.setFillColor(sf::Color::Red);
	m_shape.setOutlineColor(sf::Color::White);
	m_shape.setOutlineThickness(2.0f);
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