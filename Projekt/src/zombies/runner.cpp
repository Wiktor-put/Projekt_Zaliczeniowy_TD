#include "runner.h"
#include "Config.h"
#include <cmath>
#include "resourcemanager.h"


Runner::Runner(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    // Statystyki z Config.h
    hp           = Config::Runner::HP;
    maxHp        = Config::Runner::HP;
    baseSpeed    = Config::Runner::SPEED;
    currentSpeed = Config::Runner::SPEED;
    reward       = Config::Runner::REWARD;
    lifeCost     = Config::Runner::LIFECOST;
    lifeCost     = Config::Runner::LIFECOST;

    // --- POBIERANIE TEKSTUR ---
    texDown  = &ResourceManager::getTexture(Config::Assets::RUNNER_DOWN);
    texUp    = &ResourceManager::getTexture(Config::Assets::RUNNER_UP);
    texRight = &ResourceManager::getTexture(Config::Assets::RUNNER_RIGHT);
    texLeft  = &ResourceManager::getTexture(Config::Assets::RUNNER_LEFT);

    sprite.setTexture(*texDown);

    int fW = texDown->getSize().x / totalFrames;
    int fH = texDown->getSize().y;

    sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
    sprite.setOrigin(fW / 2.f, fH / 2.f);
    sprite.setPosition(position);
    sprite.setScale(2.1f, 2.1f); // Dajemy mu podobny rozmiar

}

void Runner::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    Zombie::update(dt, objects);

    if (!reachedEnd()) {

        // --- WYBÓR KIERUNKU ---
        if (std::abs(velocity.x) > std::abs(velocity.y)) {
            if (velocity.x > 0) sprite.setTexture(*texRight);
            else sprite.setTexture(*texLeft);
        } else {
            if (velocity.y > 0) sprite.setTexture(*texDown);
            else sprite.setTexture(*texUp);
        }

        // --- SZYBKA ANIMACJA ---
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

    sprite.setPosition(position);
}

void Runner::render(sf::RenderWindow& window) {
    window.draw(sprite);
    Zombie::render(window); // Rysuje pasek HP
}