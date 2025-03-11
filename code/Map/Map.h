#pragma once
#include <SFML/Graphics.hpp>

class Map {
	sf::RectangleShape floor;
	std::vector<sf::RectangleShape> bridges;
public:
	Map(sf::Vector2f windowSize);
	void draw(sf::RenderWindow* window);

	sf::FloatRect getFloorRect() const;
};

Map::Map(sf::Vector2f windowSize) {
	floor.setSize(sf::Vector2f(windowSize.x * 3.f, windowSize.y / 20.f));
	floor.setOrigin(floor.getSize().x / 2.f, floor.getSize().y / 2.f);
	floor.setPosition(windowSize.x / 2.f, windowSize.y);
}

void Map::draw(sf::RenderWindow* window) {
	window->draw(floor);
	for (auto bridg : bridges) {
		window->draw(bridg);
	}
}

sf::FloatRect Map::getFloorRect() const{
	return floor.getGlobalBounds();
}