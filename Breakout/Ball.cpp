#include "Ball.hpp"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~constructor~~~~~~~~~~~~~~~~~~~~~~~~~~//

Ball::Ball(float startX, float startY) {
	setPosition({ startX, startY });

	m_shape.setRadius(12.0f);
	m_shape.setFillColor(sf::Color::White);

	m_shape.setOrigin(m_shape.getGeometricCenter());

	m_speed = 3.0f;
	m_velocity = normalize({ -1.f, -1.f }) * m_speed;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~helper functions~~~~~~~~~~~~~~~~~~~~~~~~//

sf::Vector2f Ball::getPosition() const { return sf::Transformable::getPosition(); }

sf::FloatRect Ball::getGlobalBounds() const { return getTransform().transformRect(m_shape.getGlobalBounds()); }

sf::Vector2f Ball::normalize(const sf::Vector2f& source) {

	//calculates the lenght of the vector
	float length = std::sqrt((source.x * source.x) + (source.y * source.y));

	if (length != 0)
		//make the vector lenght equal to 1
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


void Ball::update(const Paddle& paddle) {
	move(m_velocity);
	
	if (getPosition().x < 16.0f)
		m_velocity.x = - m_velocity.x;

		else if (getPosition().x > 960.0f - 16.0f)
			m_velocity.x = - m_velocity.x;


	if (getPosition().y < 16.0f)
		m_velocity.y = -m_velocity.y;

	else if (getPosition().y > 800.0f + 120.0f) {
		m_velocity.y = -m_velocity.y;
		setPosition({ 960.0f / 2, 800.0f / 2 });
	}

	sf::FloatRect ballBounds = getGlobalBounds();
	sf::FloatRect paddleBounds = paddle.getGlobalBounds();

	if (ballBounds.findIntersection(paddleBounds)) {

		//calculate where on the paddle did the ball hit
		float intersectX = getPosition().x - paddle.getPosition().x;

		//normalize the result in a (-1, 1) range
		float normalizedIntersect = intersectX / (paddle.getSize().x / 2.f);

		//create a vector that has the correct angle but has weird speed
		sf::Vector2f bounceDirection(normalizedIntersect, -1.0f);		//since the x is between (-1, 1) we can t have a greater angle than 45 degrees
																		//can increase the angle by amplyfing the x with a bounce factor or reduce the y

		//normalize the vector so that we keed its direction but adjust the speed
		m_velocity = normalize(bounceDirection) * m_speed;

	}
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_shape, states);
}