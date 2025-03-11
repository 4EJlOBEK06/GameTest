#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet/BulletSystem.h"

class Weapon {
protected:
    sf::Vector2f position;  // Позиция оружия

    std::vector<std::unique_ptr<sf::Shape>> shapes;

public:
    Weapon(sf::Vector2f position) : position(position) {}

    virtual ~Weapon() = default;

    virtual void shoot(BulletSystem* bulletSystem, const float& deltaTime) = 0;

    virtual void setRotation(sf::Vector2f bodyPosition, sf::Vector2f mousePosition) = 0;

    void setPosition(sf::Vector2f bodyPosition);

    sf::Vector2f getPosition();

    void draw(sf::RenderWindow* window);
};

void Weapon::setPosition(sf::Vector2f pos) {
    sf::Vector2f offset = pos - position;
    position = pos;
    for (auto& shape : shapes) {
        shape->move(offset);
    }
}

sf::Vector2f Weapon::getPosition() {
    return position; 
}

void Weapon::draw(sf::RenderWindow* window) {
    for (const auto& shape : shapes) {
        window->draw(*shape);
    }
} 