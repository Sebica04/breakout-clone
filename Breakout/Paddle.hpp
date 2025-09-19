#pragma once
#include <SFML/Graphics.hpp>

class Paddle : public sf::Drawable, public sf::Transformable {
public:
	Paddle(float startX, float startY);

	void update();
	void onBallHit();

	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;

private:
	void updateSprite();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::RectangleShape m_hitbox;

	int m_currentState;
	sf::Clock m_hitTimer;
	bool m_isHit;

	float m_speed;
};