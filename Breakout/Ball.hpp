#pragma once
#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable, public sf::Transformable {
	
public:
	Ball(float startX, float startY);

	void update();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
};