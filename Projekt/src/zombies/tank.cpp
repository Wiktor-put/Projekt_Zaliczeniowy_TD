#include "tank.h"
#include "Config.h"

// Inicjalizacja zmiennych statycznych
sf::Texture Tank::texDown;
sf::Texture Tank::texUp;
sf::Texture Tank::texRight;
sf::Texture Tank::texLeft;
bool Tank::areTexturesLoaded = false;

Tank::Tank(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    hp           = Config::Tank::HP;
    maxHp        = Config::Tank::HP;
    baseSpeed    = Config::Tank::SPEED;
    currentSpeed = Config::Tank::SPEED;
    reward       = Config::Tank::REWARD;
    lifeCost     = Config::Tank::LIFECOST;

    // 1. Ładujemy grafiki Tanka
    if (!areTexturesLoaded) {
        bool ok = true;
        ok &= texDown.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::TANK_DOWN);
        ok &= texUp.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::TANK_UP);
        ok &= texRight.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::TANK_RIGHT);
        ok &= texLeft.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::TANK_LEFT);

        if (ok) areTexturesLoaded = true;
        else std::cerr << "Blad ladowania grafik Tanka!" << std::endl;
    }

    if (areTexturesLoaded) {
        sprite.setTexture(texDown);

        int fW = texDown.getSize().x / totalFrames;
        int fH = texDown.getSize().y;

        sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
        sprite.setOrigin(fW / 2.f, fH / 2.f);
        sprite.setPosition(position);

        // POTĘŻNA SKALA - Tank musi być dużo większy od reszty!
        sprite.setScale(3.5f, 3.5f);
    }

    // Ukrywamy stary, magentowy kwadrat
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(25.f, 25.f);
    shape.setPosition(position);
}


void Tank::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
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

    sprite.setPosition(position);
}

void Tank::render(sf::RenderWindow& window) {
    if (areTexturesLoaded) window.draw(sprite);
    Zombie::render(window); // Rysuje pasek HP na górze
}