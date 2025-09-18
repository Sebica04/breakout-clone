#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.hpp"

class Ball : public sf::Drawable, public sf::Transformable {
	
public:
	Ball(float startX, float startY);

	void update(const Paddle& paddle);
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f normalize(const sf::Vector2f& source);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
	float m_speed;
};