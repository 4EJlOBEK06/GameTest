#pragma once
#include <SFML/Graphics.hpp>
#include "Weapon/Weapon.h"

const float gravity = 9.81f;

class Player {
    sf::CircleShape head{};
    sf::ConvexShape body{};
    float velocityY{};

    sf::Vector2u windowSize;
public:

	Player(sf::Vector2u windowSize, float height = 90.f, float width = 40.f, sf::Color color = sf::Color::White);
    void playerGravity(sf::FloatRect floor, float deltaTime);
    void jump(sf::FloatRect floor);
    void animationPlayer(float timeSec);
    void movePlayer(float dx, float dy);
    void draw(sf::RenderWindow* window);

    sf::Vector2f getBodyPosition();
    sf::Vector2f getLegsPosition();
};

Player::Player(sf::Vector2u windowSize, float height, float width, sf::Color color) : windowSize(windowSize) {

    head.setRadius(height / 7.f);
    head.setFillColor(color);
    head.setOrigin(head.getRadius(), head.getRadius());

    body.setPointCount(3);
    body.setPoint(0, sf::Vector2f(0.f, 0.f));                    // Левый вверх
    body.setPoint(1, sf::Vector2f(width, 0.f));                  // Правый вверх
    body.setPoint(2, sf::Vector2f(width / 2.f, height / 1.8f));  // Центр низ
    body.setFillColor(color);
    body.setOrigin((body.getPoint(0).x + body.getPoint(1).x) / 2.f, (body.getPoint(0).y + body.getPoint(2).y) / 2.f);

    body.setPosition(windowSize.x / 2, windowSize.y - windowSize.y / 3);
    head.setPosition(body.getPosition().x, body.getPosition().y - head.getRadius() * 3.5f);
}

void Player::playerGravity(sf::FloatRect floor, float deltaTime) {
    bool onGround = floor.contains(getLegsPosition());

    if (!onGround) {
        velocityY += gravity * deltaTime * 100.f;
    }
    else if (velocityY > 0) {
        velocityY = 0;
    }

    movePlayer(0.f, velocityY * deltaTime);
}

void Player::jump(sf::FloatRect floor) {
    if (floor.contains(getLegsPosition())) {
        velocityY -= 700.f;
    }
}

void Player::animationPlayer(float timeSec) {
    timeSec *= 15;
}

void Player::movePlayer(float dx, float dy) {
    sf::Vector2f newPosition = body.getPosition() + sf::Vector2f(dx, dy);
    if (newPosition.x < 0 || newPosition.x > windowSize.x) {
        dx = 0;
    }
    head.move(dx, dy);
    body.move(dx, dy);
}

void Player::draw(sf::RenderWindow* window) {
    window->draw(body);
    window->draw(head);
}

sf::Vector2f Player::getBodyPosition() {
    return body.getPosition();
}

sf::Vector2f Player::getLegsPosition() {
    return sf::Vector2f(getBodyPosition().x, (body.getPoint(0).y + body.getPoint(2).y) / 1.5f + getBodyPosition().y);
}