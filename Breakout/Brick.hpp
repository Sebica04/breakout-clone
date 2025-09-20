#pragma once 
#include <SFML/Graphics.hpp>


class Brick : public sf::Drawable, public sf::Transformable {

public:
	
	Brick(float startX, float starty);

	sf::FloatRect getGlobalBounds() const;

	void destroy();

	bool isDestroyed() const;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RectangleShape m_shape;
	bool m_isDestroyed;
};