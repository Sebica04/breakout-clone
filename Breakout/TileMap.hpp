#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


class TileMap : public sf::Drawable, public sf::Transformable {
	
public:
	bool load(const std::string& tilemapJsonPath, const std::string& tilesetPath);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::VertexArray m_vertices;
	sf::Texture     m_tileset;
};