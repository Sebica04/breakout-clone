#include "Ball.hpp"

Ball::Ball(float startX, float startY) {
	setPosition({ startX, startY });

	m_shape.setRadius(12.0f);
	m_shape.setFillColor(sf::Color::White);

	m_shape.setOrigin(m_shape.getGeometricCenter());

	m_velocity.x = -2.0f;
	m_velocity.y = -2.0f;
}

void Ball::update() {
	move(m_velocity);
	
	if (getPosition().x < 16.0f)
		m_velocity.x = - m_velocity.x;

		else if (getPosition().x > 960.0f - 16.0f)
			m_velocity.x = - m_velocity.x;


	if (getPosition().y < 16.0f)
		m_velocity.y = - m_velocity.y;

		else if (getPosition().y > 800.0f - 16.0f)
			m_velocity.y = - m_velocity.y;
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_shape, states);
}