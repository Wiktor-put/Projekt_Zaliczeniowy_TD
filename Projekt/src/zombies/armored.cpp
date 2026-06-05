#include "armored.h"
#include "Config.h"
#include <cmath>

sf::Texture Armored::texDown;
sf::Texture Armored::texUp;
sf::Texture Armored::texRight;
sf::Texture Armored::texLeft;
bool Armored::areTexturesLoaded = false;

Armored::Armored(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    hp           = Config::Armored::HP;
    maxHp        = Config::Armored::HP;
    baseSpeed    = Config::Armored::SPEED;
    currentSpeed = Config::Armored::SPEED;
    reward       = Config::Armored::REWARD;
    lifeCost     = Config::Armored::LIFECOST;

    // 1. Ładujemy grafiki z toporem z Configu
    if (!areTexturesLoaded) {
        bool ok = true;
        ok &= texDown.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::ARMORED_DOWN);
        ok &= texUp.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::ARMORED_UP);
        ok &= texRight.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::ARMORED_RIGHT);
        ok &= texLeft.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::ARMORED_LEFT);

        if (ok) areTexturesLoaded = true;
        else std::cerr << "Blad ladowania grafik Armored Zombie!" << std::endl;
    }

    if (areTexturesLoaded) {
        sprite.setTexture(texDown);

        // Wyliczamy wymiary jednej klatki (szerokość dzielona na 8)
        int fW = texDown.getSize().x / totalFrames;
        int fH = texDown.getSize().y;

        sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
        sprite.setOrigin(fW / 2.f, fH / 2.f);
        sprite.setPosition(position);
        sprite.setScale(2.2f, 2.2f); // Dostosuj tę skalę, by zombiak wyglądał potężnie
    }

    // Szary, nieco większy kwadrat
    shape.setSize(sf::Vector2f(35.f, 35.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(17.5f, 17.5f);
    shape.setPosition(position);
}

// Opancerzony zombiak redukuje obrażenia od kul!
void Armored::takeDamage(int dmg, DamageType type) {
    if (type == DamageType::BULLET) {
        // Redukcja obrażeń (np. 0.5f, czyli 50% mniej)
        float reducedDmg = static_cast<float>(dmg) * (1.0f - Config::Armored::BULLET_RESISTANCE);

        // Wywołanie oryginalnej funkcji z bazowej klasy, ale z mniejszymi obrażeniami
        Zombie::takeDamage(static_cast<int>(reducedDmg), type);
    } else {
        // Inne obrażenia (np. ogień z miotacza lub wybuch) zadają pełne obrażenia
        Zombie::takeDamage(dmg, type);
    }
}

void Armored::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
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

        // --- ANIMACJA  ---
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

void Armored::render(sf::RenderWindow& window) {
    if (areTexturesLoaded) window.draw(sprite);
    Zombie::render(window);
}