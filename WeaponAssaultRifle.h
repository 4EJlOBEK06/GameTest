#pragma once
#include <cmath>
#include "Bullet/BulletSystem.h"

#define M_PI 3.14159265358979323846

class AssaultRifle : public Weapon {
    const float RateFire{ 0.15f };
    float countTime{};
public:
    AssaultRifle(sf::Vector2f position);
    void shoot(BulletSystem* bulletSystem, const float& deltaTime) override;
    void setRotation(sf::Vector2f bodyPosition, sf::Vector2f mousePosition) override;
};

AssaultRifle::AssaultRifle(sf::Vector2f position) : Weapon(position) {

    // Ствол
    auto barrel = std::make_unique<sf::RectangleShape>(sf::Vector2f(62.f, 3.f));
    barrel->setFillColor(sf::Color(125, 127, 125));
    barrel->setPosition(position.x, position.y - 10.f);

    barrel->setOrigin(0.f, barrel->getSize().y / 2.f);

    // Цевье
    auto handguard = std::make_unique<sf::RectangleShape>(sf::Vector2f(18.f, 6.f));
    handguard->setFillColor(sf::Color(100, 50, 50));
    handguard->setPosition(position.x + 35.f, position.y - 12.f);

    // Ствольный короб
    auto receiver = std::make_unique<sf::RectangleShape>(sf::Vector2f(35.f, 5.f));
    receiver->setFillColor(sf::Color(125, 127, 125));
    receiver->setPosition(position.x, position.y - 12.f);

    // Магазин
    auto magazine = std::make_unique<sf::RectangleShape>(sf::Vector2f(8.f, 16.f));
    magazine->setFillColor(sf::Color(100, 50, 50));
    magazine->setPosition(position.x + 22.f, position.y - 8.f);

    // Рукоятка
    auto grip = std::make_unique<sf::RectangleShape>(sf::Vector2f(7.f, 16.f));
    grip->setFillColor(sf::Color(73, 66, 61));
    grip->setPosition(position.x, position.y - 12.f);

    shapes.push_back(std::move(barrel));
    shapes.push_back(std::move(handguard));
    shapes.push_back(std::move(grip));
    shapes.push_back(std::move(receiver));
    shapes.push_back(std::move(magazine));
}

void AssaultRifle::shoot(BulletSystem* bulletSystem, const float& deltaTime) {
    countTime += deltaTime;
    if (countTime >= RateFire) {

        sf::Vector2f barrelPosition = shapes[0].get()->getPosition();
        float angle = shapes[0].get()->getRotation() * M_PI / 180.f;

        sf::Vector2f offset(cos(angle) * shapes[0].get()->getLocalBounds().getSize().x, sin(angle) * shapes[0].get()->getLocalBounds().getSize().x);
        sf::Vector2f position = barrelPosition + offset;

        bulletSystem->spawnBullet(position, 1, 3.f, shapes[0].get()->getRotation(), 2000.f, 5.f, 0.5f, 0.7f);

        countTime = 0.f;
    }
}

void AssaultRifle::setRotation(sf::Vector2f bodyPosition, sf::Vector2f mousePosition) {
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

    // Вычисляем позицию цевья
    float handguardXOffset{35.f}, handguardYOffset{-2.f * scaleY};

    sf::Vector2f handguardOffset = { handguardXOffset * cos(angleRad) + handguardYOffset * (-sin(angleRad)), handguardXOffset * sin(angleRad) + handguardYOffset * cos(angleRad) };
    shapes[1]->setPosition(basePos + handguardOffset);

    // Вычисляем позицию магазина
    float magazineXOffset{ 22.f }, magazineYOffset{ 2.f * scaleY};

    sf::Vector2f magazineOffset = { magazineXOffset * cos(angleRad) + magazineYOffset * (-sin(angleRad)), magazineXOffset * sin(angleRad) + magazineYOffset * cos(angleRad) };
    shapes[4]->setPosition(basePos + magazineOffset);
}