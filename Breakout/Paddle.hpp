#pragma once
#include <SFML/Graphics.hpp>

class Paddle : public sf::Drawable, public sf::Transformable {
public:
	Paddle(float startX, float startY);

	void update();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::RectangleShape m_shape;
	float m_speed;
};