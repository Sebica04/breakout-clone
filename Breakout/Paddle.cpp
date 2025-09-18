#include "Paddle.hpp"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~constructor~~~~~~~~~~~~~~~~~~~~~~~~~~//


Paddle::Paddle(float startX, float startY) {
	
	setPosition({ startX, startY });

	m_shape.setSize({120.0f, 16.0f});
	m_shape.setFillColor(sf::Color::White);
	m_shape.setOrigin(m_shape.getGeometricCenter());

	m_speed = 4.0f;

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~helper functions~~~~~~~~~~~~~~~~~~~~~~~~//

sf::Vector2f Paddle::getPosition() const { return sf::Transformable::getPosition(); }

sf::Vector2f Paddle::getSize() const { return m_shape.getSize(); }

sf::FloatRect Paddle::getGlobalBounds() const { return getTransform().transformRect(m_shape.getGlobalBounds()); }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//



void Paddle::update() {


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		move({-m_speed, 0.f});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		move({ m_speed, 0.f });
	}

	if (getPosition().x < 70) {
		setPosition({ 71, getPosition().y });
	}
	else if (getPosition().x > 960 - 70) {
		setPosition({ 960 - 69, getPosition().y });
	}
}

void Paddle::draw(sf::RenderTarget& target, sf::RenderStates states)const {
	states.transform *= getTransform();
	target.draw(m_shape, states);
}