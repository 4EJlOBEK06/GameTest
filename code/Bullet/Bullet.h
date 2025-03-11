#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
    sf::CircleShape bullet{};
    float angle{};
    float speed{};
    float lifetime{};

    float velocityY{}, velocityX{};
    float velocityLossCoefficient{};
public:
    Bullet(sf::Vector2f bulletPosition, float bulletSize, float angleShot, float bulletSpeed, float bulletLifetime, float velocityLossCoefficient = 1.f);

    friend class BulletSystem;
};

Bullet::Bullet(sf::Vector2f bulletPosition, float bulletSize, float angleShot, float bulletSpeed, float bulletLifetime, float velocityLossCoefficient)
    : bullet(bulletSize / 2), angle(angleShot), speed(bulletSpeed), lifetime(bulletLifetime), velocityLossCoefficient(velocityLossCoefficient) {
    bullet.setOrigin(bullet.getRadius() / 2.f, bullet.getRadius() / 2.f);
    bullet.setPosition(bulletPosition);
};