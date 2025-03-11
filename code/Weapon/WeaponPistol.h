#pragma once
#include "Bullet/BulletSystem.h"

#define M_PI 3.14159265358979323846

class Pistol : public Weapon {
    const float RateFire{ 0.5f };
    float countTime{};
public:
    Pistol(sf::Vector2f position);
    void shoot(BulletSystem* bulletSystem, const float& deltaTime) override;
    void setRotation(sf::Vector2f bodyPosition, sf::Vector2f mousePosition) override;
};

Pistol::Pistol(sf::Vector2f position) : Weapon(position) {

    // Ствол
    auto barrel = std::make_unique<sf::RectangleShape>(sf::Vector2f(35.f, 8.f));
    barrel->setFillColor(sf::Color(125, 127, 125));
    barrel->setPosition(position.x, position.y - 10.f);

    barrel->setOrigin(0.f, barrel->getSize().y / 2.f);

    // Рукоятка
    auto grip = std::make_unique<sf::RectangleShape>(sf::Vector2f(9.f, 18.f));
    grip->setFillColor(sf::Color(73, 66, 61));
    grip->setPosition(position.x, position.y - 14.f);

    shapes.push_back(std::move(barrel));
    shapes.push_back(std::move(grip));
}

void Pistol::shoot(BulletSystem* bulletSystem, const float& deltaTime) {
    countTime += deltaTime;
    if (countTime >= RateFire) {

        sf::Vector2f barrelPosition = shapes[0].get()->getPosition();
        float angle = shapes[0].get()->getRotation() * M_PI / 180.f;

        sf::Vector2f offset(cos(angle) * shapes[0].get()->getLocalBounds().getSize().x, sin(angle) * shapes[0].get()->getLocalBounds().getSize().x);
        sf::Vector2f position = barrelPosition + offset;

        bulletSystem->spawnBullet(position, 1, 4.f, shapes[0].get()->getRotation(), 1500.f, 3.f, 0.5f, 0.8f);

        countTime = 0.f;
    }
}

void Pistol::setRotation(sf::Vector2f bodyPosition, sf::Vector2f mousePosition) {
    float angle = atan2(mousePosition.y - bodyPosition.y, mousePosition.x - bodyPosition.x) * 180.0f / M_PI;
    bool flipped = (angle > 90.f || angle < -90.f);

    if (flipped) {
        if (angle < 120.f && angle > 0.f) {
            angle = 120.f;
        }
        else if (angle > -120.f && angle < 120.f) {
            angle = -120.f;
        }
    }
    else {
        if (angle < -60.f) {
            angle = -60.f;
        }
        else if (angle > 60.f) {
            angle = 60.f;
        }
    }

    float scaleY = flipped ? -1.f : 1.f;

    for (auto& shape : shapes) {
        shape->setRotation(angle);
        shape->setScale(1, scaleY);
    }

    float angleRad = angle * (M_PI / 180.f);
    sf::Vector2f basePos = shapes[0]->getPosition();

    // Вычисляем позицию рукояти
    float height = shapes[0]->getLocalBounds().height - 12.f;
    sf::Vector2f gripOffset = {
        -height * sin(angleRad),
        height * cos(angleRad)
    };

    if (flipped) gripOffset = -gripOffset;  // Корректируем при отражении

    shapes[1]->setPosition(basePos + gripOffset);
}