

#include "../includes/map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <tinyxml2.h>


Map::Map(const std::string &tmxFilePath , const std::string& tilesetPath , float tileSize) : tileSize(tileSize) , scale(1.0f) {
	if(!this->tilesetTexture.loadFromFile(tilesetPath)) {
		std::cerr << "failed to load tileset texture!" << '\n';
	}
	else {
		std::cout << "tileset loaded" << '\n';
	}

	tinyxml2::XMLDocument doc;

	if(doc.LoadFile(tmxFilePath.c_str()) != tinyxml2::XML_SUCCESS) {
		std::cerr << "Failed to load map file in xml" << '\n';
		return;
	}

	tinyxml2::XMLElement *mapElement = doc.FirstChildElement("map");
	int width = mapElement->IntAttribute("width");
	int height = mapElement->IntAttribute("height");

	this->vertices.setPrimitiveType(sf::Quads);
	int totalTiles = width * height;

	this->vertices.resize(totalTiles * 4);

	int tileCounter = 0;


	for(tinyxml2::XMLElement* layer = mapElement->FirstChildElement("layer") ;   layer != nullptr ; layer = layer->NextSiblingElement("layer")) {
		std::string layerName = layer->Attribute("name");
		bool isCollidable =
		(layerName == "bounding walls" || layerName == "inner walls" ||
		 layerName == "wall objects" || layerName == "Floor objects");
		
		tinyxml2::XMLElement *dataElement = layer->FirstChildElement("data");
		const char* encodedData = dataElement->GetText();


		if(!encodedData) continue;


		std::vector<u_int> tileIndices;
		std::stringstream ss(encodedData);
		std::string tile;

		while(std::getline(ss , tile , ',')) {
			tileIndices.push_back(std::stoi(tile));
		}


		for(int i = 0 ; i < tileIndices.size() ; ++i) {
			u_int tileID = tileIndices[i];

			if(tileID == 0) continue;

			int tilePerRow = this->tilesetTexture.getSize().x / this->tileSize;
			int tr = (tileID - 1) % tilePerRow;
			int tc = (tileID - 1) / tilePerRow;


			int x = (i %  width) * this->tileSize;

			int y = (i / width) * this->tileSize;



		        if (tileCounter * 4 >= vertices.getVertexCount()) {
				vertices.resize(vertices.getVertexCount() + 4);
			}


			sf::Vertex *quad = &this->vertices[tileCounter * 4];

			quad[0].position = sf::Vector2f(x , y);
			quad[1].position = sf::Vector2f(x + tileSize , y);
			quad[2].position = sf::Vector2f(x + tileSize , y + tileSize);
			quad[3].position = sf::Vector2f(x , y + tileSize);


			quad[0].texCoords = sf::Vector2f(tr * this->tileSize, tc * this->tileSize);
			quad[1].texCoords = sf::Vector2f((tr + 1) * this->tileSize, tc * this->tileSize);
			quad[2].texCoords = sf::Vector2f((tr + 1) * this->tileSize, (tc + 1) * this->tileSize);
			quad[3].texCoords = sf::Vector2f(tr * this->tileSize, (tc + 1) * this->tileSize);



			if(isCollidable) {
				this->originalCollisionRects.emplace_back(x  , y ,tileSize  , tileSize);
			}

			tileCounter++;

		}



 
	}

		this->collisionRects = this->originalCollisionRects;


}

void Map::scaleToFit(const sf::RenderWindow& window) {

	sf::Vector2u windowSize = window.getSize();
	sf::FloatRect mapBounds(0.f , 0.f , this->vertices.getBounds().width , this->vertices.getBounds().height);

	float scaleX = static_cast<float>(windowSize.x) / mapBounds.width;

	float scaleY= static_cast<float>(windowSize.y) / mapBounds.height;

	this->scale = std::min(scaleX , scaleY);

	sf::VertexArray scaledVertices = this->vertices;

	for(int i = 0 ; i < scaledVertices.getVertexCount() ; i++) {
		scaledVertices[i].position *= scale;
	}

	this->collisionRects.clear();

	for(auto & it : this->originalCollisionRects) {
		sf::FloatRect updated(it.left * scale , it.top * scale , it.width * scale , it.height * scale);

		this->collisionRects.push_back(updated);
	}

	this->scaledVerticesForRendering = scaledVertices;



}


void Map::render(sf::RenderWindow& window) {
	this->scaleToFit(window);

	sf::RenderStates states;

	states.texture = &this->tilesetTexture;

	window.draw(this->scaledVerticesForRendering , states);
}

bool Map::isColliding(const sf::FloatRect & playerBounds) const {
	for(auto it : this->collisionRects) {
		if(it.intersects(playerBounds)) return true;
	}
	return false;
}

sf::FloatRect Map::getMapBounds() const {
	float mapWidth = this->vertices.getBounds().width * this->scale;
	float mapHeight = this->vertices.getBounds().height * this->scale;

	return sf::FloatRect(0.f , 0.f , mapWidth , mapHeight);

}

float Map::getScale() const { return scale;  }

