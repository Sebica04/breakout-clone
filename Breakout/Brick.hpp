#pragma once 
#include <SFML/Graphics.hpp>


class Brick : public sf::Drawable, public sf::Transformable {

public:
	
	Brick(float startX, float starty, const sf::Color& color);

	sf::FloatRect getGlobalBounds() const;

	void destroy();

	bool isDestroyed() const;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RectangleShape m_shape;
	bool m_isDestroyed;

	sf::Color m_color;
	static sf::Texture s_texture;
	static bool s_textureLoaded;
};