#pragma once
#include <SFML/Graphics.hpp>
#include "Player/Player.h"
#include "Map/Map.h"
#include "Bullet/BulletSystem.h"
#include "Weapon/WeaponPistol.h"
#include "Weapon/WeaponShotgun.h"
#include "Weapon/WeaponAssaultRifle.h"

class MainGame {
    sf::RenderWindow window;
    Player player;
    Map map;
    BulletSystem bulletSystem;

    Pistol pistol;
    Shotgun shotgun;
    AssaultRifle assaultRifle;

    std::vector<Weapon*> currentWeapon;

    sf::Clock sfClock;
    sf::Time sfTime;

    sf::Time timeSwapWeapon;
    sf::Time timeShoot;
    sf::Time timeJump;

    int indexWeapon{ 0 };
    const float speedPlayer{ 700.f };

    void handleInput(sf::Time time);
public:
    MainGame();
    void run();
};

MainGame::MainGame()
    : window(sf::VideoMode(1680, 720), "GameTest"),
    player(window.getSize()),
    map(sf::Vector2f(window.getSize())),
    pistol(player.getBodyPosition()),
    shotgun(player.getBodyPosition()),
    assaultRifle(player.getBodyPosition()) {

    srand(static_cast<unsigned>(time(nullptr)));

    window.setFramerateLimit(1000);

    currentWeapon = { &pistol, &shotgun, &assaultRifle };
}

void MainGame::run() {
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                currentWeapon[indexWeapon]->setRotation(player.getBodyPosition(), window.mapPixelToCoords(pixelPos));
            }
        }
        handleInput(sfTime);
        player.playerGravity(map.getFloorRect(), sfTime.asSeconds());
        currentWeapon[indexWeapon]->setPosition(player.getBodyPosition());
        bulletSystem.update(sfTime.asSeconds(), map.getFloorRect());

        window.clear();
        player.draw(&window);
        map.draw(&window);
        bulletSystem.drawBullets(&window);
        currentWeapon[indexWeapon]->draw(&window);
        window.display();

        sfTime = sfClock.restart();
    }
}

void MainGame::handleInput(sf::Time time) {
    timeSwapWeapon += time;
    timeShoot += time;
    timeJump += time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.movePlayer(-time.asSeconds() * speedPlayer, 0.f);  // Влево
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.movePlayer(time.asSeconds() * speedPlayer, 0.f);  // Вправо
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && timeSwapWeapon.asMilliseconds() > 150) {
        ++indexWeapon;
        if (indexWeapon + 1 > currentWeapon.size()) { indexWeapon = 0; }
        timeSwapWeapon = sf::Time::Zero;
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::F) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) && timeShoot.asMilliseconds() > 50) {
        currentWeapon[indexWeapon]->shoot(&bulletSystem, timeShoot.asSeconds());
        timeShoot = sf::Time::Zero;
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && timeJump.asMilliseconds() > 100) {
        player.jump(map.getFloorRect());
        timeJump = sf::Time::Zero;
    }
}
