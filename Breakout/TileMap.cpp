#include "TileMap.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool TileMap::load(const std::string& tilemapJsonPath, const std::string& tilesetPath) {
	

	//~~~~~~~~~~~~~load the tileset image~~~~~~~~~~~

	if (!m_tileset.loadFromFile(tilesetPath)) {
		std::cerr << "Error: Failed to load the tileset image: " << tilesetPath << std::endl;
		return false;
	}


	//~~~~~~~~~~~load and parse the JSON file~~~~~~~~
	
	json mapData;

	std::ifstream file(tilemapJsonPath);

	if (!file.is_open()) {
		std::cerr << "Error: Failed to open tilemap file: " << tilemapJsonPath << std::endl;
		return false;
	}

	file >> mapData;

	int tileWidth = mapData["tilewidth"];
	int tileHeight = mapData["tileheight"];
	
	const auto& tiles = mapData["layers"][0]["tiles"];

	m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
	m_vertices.resize(tiles.size() * 6);

	int tilesPerRow = m_tileset.getSize().x / tileWidth;

	for (size_t i = 0; i < tiles.size(); ++i) {
		const auto& tile = tiles[i];
		int tileId = tile["tile"];
		int tileX = tile["x"];
		int tileY = tile["y"];
		int rot = tile["rot"];
		
		int tu = tileId % tilesPerRow;
		int tv = tileId / tilesPerRow;

		sf::Vertex* tri = &m_vertices[i * 6];

		//points on the window 
		sf::Vector2f p1(tileX * tileWidth, tileY * tileHeight);				//top left
		sf::Vector2f p2((tileX + 1) * tileWidth, tileY * tileHeight);		//top right
		sf::Vector2f p3((tileX + 1) * tileWidth, (tileY + 1) * tileHeight);	//bot right
		sf::Vector2f p4(tileX * tileWidth, (tileY + 1) * tileHeight);		//bot left

		//point on the tileset
		sf::Vector2f t1(tu * tileWidth, tv * tileHeight);				//top left
		sf::Vector2f t2((tu + 1) * tileWidth, tv * tileHeight);			//top right
		sf::Vector2f t3((tu + 1) * tileWidth, (tv + 1) * tileHeight);	//bot right
		sf::Vector2f t4(tu * tileWidth, (tv + 1) * tileHeight);			//bot left


		//setting the texture for rotation
		switch (rot) {
		case 1:
			tri[0].texCoords = t4; tri[1].texCoords = t1; tri[2].texCoords = t2;
			tri[3].texCoords = t2; tri[4].texCoords = t3; tri[5].texCoords = t4;
			break;
		case 2:
			tri[0].texCoords = t3; tri[1].texCoords = t4; tri[2].texCoords = t1;
			tri[3].texCoords = t1; tri[4].texCoords = t2; tri[5].texCoords = t3;
			break;
		case 3:
			tri[0].texCoords = t2; tri[1].texCoords = t3; tri[2].texCoords = t4;
			tri[3].texCoords = t4; tri[4].texCoords = t1; tri[5].texCoords = t2;
			break;
		default:
			tri[0].texCoords = t1; tri[1].texCoords = t2; tri[2].texCoords = t3;
			tri[3].texCoords = t3; tri[4].texCoords = t4; tri[5].texCoords = t1;
			break;
		}

		//seting the position of the triangles
		tri[0].position = p1; tri[1].position = p2; tri[2].position = p3;
		tri[3].position = p3; tri[4].position = p4; tri[5].position = p1;
	}
	return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &m_tileset;
	target.draw(m_vertices, states);
}