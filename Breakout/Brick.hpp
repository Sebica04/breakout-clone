#pragma once 
#include <SFML/Graphics.hpp>


class Brick : public sf::Drawable, public sf::Transformable {

public:

	enum class Type { Normal, Sturdy, Steel };
	
	Brick(float startX, float starty,Type, const sf::Color& color);

	sf::FloatRect getGlobalBounds() const;
	
	void takeDamage();

	int health();

	void destroy();

	bool isDestroyed() const;

	Type getType() const { return m_type; }

	

private:
	void updateTexture();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RectangleShape m_shape;
	bool m_isDestroyed;
	int m_health;
	Type m_type;
	static sf::Texture s_texture;
	static bool s_textureLoaded;
};