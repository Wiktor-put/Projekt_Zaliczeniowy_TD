#include "tank.h"
#include "Config.h"
#include "resourcemanager.h"

Tank::Tank(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    hp           = Config::Tank::HP;
    maxHp        = Config::Tank::HP;
    baseSpeed    = Config::Tank::SPEED;
    currentSpeed = Config::Tank::SPEED;
    reward       = Config::Tank::REWARD;
    lifeCost     = Config::Tank::LIFECOST;

    // 1. Pobieramy grafiki Tanka
    texDown  = &ResourceManager::getTexture(Config::Assets::TANK_DOWN);
    texUp    = &ResourceManager::getTexture(Config::Assets::TANK_UP);
    texRight = &ResourceManager::getTexture(Config::Assets::TANK_RIGHT);
    texLeft  = &ResourceManager::getTexture(Config::Assets::TANK_LEFT);

    sprite.setTexture(*texDown);

    int fW = texDown->getSize().x / totalFrames;
    int fH = texDown->getSize().y;

    sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
    sprite.setOrigin(fW / 2.f, fH / 2.f);
    sprite.setPosition(position);
    sprite.setScale(3.1f, 3.1f); // Skala bossa!


    // Ukrywamy stary, magentowy kwadrat
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(25.f, 25.f);
    shape.setPosition(position);
}


void Tank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    Zombie::update(dt, objects);

    // Zmienna przechowująca korektę wizualną obrazka (domyślnie brak przesunięcia)
    sf::Vector2f visualOffset(0.f, 0.f);

    if (!reachedEnd()) {

        // --- WYBÓR KIERUNKU ---
        if (std::abs(velocity.x) > std::abs(velocity.y)) {
            if (velocity.x > 0) sprite.setTexture(*texRight);
            else sprite.setTexture(*texLeft);
            // KOREKTA WIZUALNA: Podnosimy obrazek do góry.

            visualOffset.y = -20.f;
        } else {
            if (velocity.y > 0) sprite.setTexture(*texDown);
            else sprite.setTexture(*texUp);
            visualOffset.y = -20.f;
        }

        // --- CIĘŻKA, WOLNA ANIMACJA ---
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrame++;

            if (currentFrame >= totalFrames) {
                currentFrame = 0;
            }

            int fW = sprite.getTexture()->getSize().x / totalFrames;
            int fH = sprite.getTexture()->getSize().y;
            sprite.setTextureRect(sf::IntRect(currentFrame * fW, 0, fW, fH));
        }
    }

    sprite.setPosition(position+ visualOffset);
}

void Tank::render(sf::RenderWindow& window) {
    window.draw(sprite);
    Zombie::render(window); // Rysuje pasek HP na górze
}