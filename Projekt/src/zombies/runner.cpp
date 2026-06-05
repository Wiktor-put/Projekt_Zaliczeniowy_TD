#include "runner.h"
#include "Config.h"

// Inicjalizacja zmiennych statycznych
sf::Texture Runner::texDown;
sf::Texture Runner::texUp;
sf::Texture Runner::texRight;
sf::Texture Runner::texLeft;
bool Runner::areTexturesLoaded = false;

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

    // 1. Ładujemy grafiki "No-axe"
    if (!areTexturesLoaded) {
        bool ok = true;
        ok &= texDown.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::RUNNER_DOWN);
        ok &= texUp.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::RUNNER_UP);
        ok &= texRight.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::RUNNER_RIGHT);
        ok &= texLeft.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::RUNNER_LEFT);

        if (ok) areTexturesLoaded = true;
        else std::cerr << "Blad ladowania grafik Runnera!" << std::endl;
    }

    if (areTexturesLoaded) {
        sprite.setTexture(texDown);

        int fW = texDown.getSize().x / totalFrames;
        int fH = texDown.getSize().y;

        sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
        sprite.setOrigin(fW / 2.f, fH / 2.f);
        sprite.setPosition(position);
        sprite.setScale(2.1f, 2.1f); // Dajemy mu podobny rozmiar
    }

    // Przezroczysty, mniejszy kwadrat
    shape.setSize(sf::Vector2f(25.f, 25.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(12.5f, 12.5f);
    shape.setPosition(position);
}

void Runner::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    Zombie::update(dt, objects);

    if (areTexturesLoaded && !reachedEnd()) {

        // --- WYBÓR KIERUNKU ---
        if (std::abs(velocity.x) > std::abs(velocity.y)) {
            if (velocity.x > 0) sprite.setTexture(texRight);
            else sprite.setTexture(texLeft);
        } else {
            if (velocity.y > 0) sprite.setTexture(texDown);
            else sprite.setTexture(texUp);
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
    if (areTexturesLoaded) window.draw(sprite);
    Zombie::render(window); // Rysuje pasek HP
}