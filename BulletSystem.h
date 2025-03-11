#pragma once
#include <functional>
#include "Bullet/Bullet.h"

class BulletSystem {
public:
	std::vector<Bullet> bullets{};
	void spawnBullet(sf::Vector2f bulletPosition, int bulletCount, float bulletSize, float angleShot, float bulletSpeed, float bulletLifetime, float bulletSpreadPercentage, float velocityLossCoefficient);
    void update(float dt, sf::FloatRect floor);
    void drawBullets(sf::RenderWindow* window);
};

void BulletSystem::spawnBullet(sf::Vector2f bulletPosition, int bulletCount, float bulletSize, float angleShot, float bulletSpeed, float bulletLifetime, float bulletSpreadPercentage, float velocityLossCoefficient = 1.f) {
    const float BulletVelocityDeviation{ 15.f };
    for (int i = 0; i < bulletCount; ++i) {

        float spreadOffset = ((rand() / (float)RAND_MAX) - 0.5f) * 2.f * bulletSpreadPercentage;
        angleShot += spreadOffset;

        float SpeedOffset = ((rand() / (float)RAND_MAX) - 0.5f) * 2.f * BulletVelocityDeviation;
        bulletSpeed += SpeedOffset;

        bullets.emplace_back(bulletPosition, bulletSize, angleShot, bulletSpeed, bulletLifetime, velocityLossCoefficient);
    }
}

void BulletSystem::update(float dt, sf::FloatRect floor) {
    const float gravity = 9.81f;

    std::vector<std::function<void()>> tasks;

    for (auto& bullet : bullets) {

        if (bullet.velocityX == 0) {  // Если это первый кадр
            float theta = bullet.angle * 3.14159265f / 180.f;
            bullet.velocityX = bullet.speed * cos(theta);
            bullet.velocityY = bullet.speed * sin(theta);
        }

        bullet.velocityY += (gravity * dt) * 10;

        bullet.velocityX *= std::pow(bullet.velocityLossCoefficient, dt);

        bullet.bullet.move(bullet.velocityX * dt, bullet.velocityY * dt);

        if (bullet.bullet.getGlobalBounds().intersects(floor)) {
            if (bullet.velocityY > 0) {
                if (bullet.bullet.getRadius() > 1.f) {
                    int fragmentCount = 3 + rand() % 3;
                    float newSize = bullet.bullet.getRadius() * 0.5f;
                    float newSpeed = bullet.speed * 0.6f;

                    tasks.push_back([this, bullet, fragmentCount, newSize, newSpeed]() {
                        spawnBullet(bullet.bullet.getPosition(), fragmentCount, newSize, bullet.angle, newSpeed, bullet.lifetime * 0.8f, 15.f, bullet.velocityLossCoefficient);
                    });

                    bullet.lifetime = 0;
                }
                bullet.velocityY = -bullet.velocityY * 0.5f;
                bullet.velocityX *= 0.7f;
            }
        }

        // Уменьшаем время жизни
        bullet.lifetime -= dt;
    }
    for (auto& task : tasks) {
        task();
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& bullet) { return bullet.lifetime <= 0; }),
        bullets.end());
}


void BulletSystem::drawBullets(sf::RenderWindow* window) {
    for (auto& bullet : bullets) {
        window->draw(bullet.bullet);
    }
}