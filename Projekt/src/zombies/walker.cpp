#include "walker.h"
#include "Config.h"
#include <cmath>

// Inicjalizacja statycznych tekstur
sf::Texture Walker::texDown;
sf::Texture Walker::texUp;
sf::Texture Walker::texRight;
sf::Texture Walker::texLeft;
bool Walker::areTexturesLoaded = false;

Walker::Walker(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints)
{
    position = waypoints[0];
    alive = true;

    hp       = Config::Walker::HP;
    maxHp    = Config::Walker::HP;
    baseSpeed    = Config::Walker::SPEED;
    currentSpeed = Config::Walker::SPEED;
    reward   = Config::Walker::REWARD;
    lifeCost = Config::Walker::LIFECOST;

    // --- MAGIA TEKSTUR ---
    // 1. Ładujemy obrazek z dysku tylko dla pierwszego zombiaka
    if (!areTexturesLoaded) {
        bool ok = true;
        ok &= texDown.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::WALKER_DOWN);
        ok &= texUp.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::WALKER_UP);
        ok &= texRight.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::WALKER_RIGHT);
        ok &= texLeft.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::WALKER_LEFT);

        if (ok) areTexturesLoaded = true;
        else std::cerr << "Blad ladowania jednej z grafik Walkera!" << std::endl;
    }

    // 2. Podpinamy wczytaną teksturę do naszego sprite'a
    if (areTexturesLoaded) {
        // Domyślnie startujemy w prawo
        sprite.setTexture(texRight);

        // Zawsze bierzemy wysokość całego paska i 1/6 jego szerokości
        int fW = texRight.getSize().x / totalFrames;
        int fH = texRight.getSize().y;

        sprite.setTextureRect(sf::IntRect(0, 0, fW, fH));
        sprite.setOrigin(fW / 2.f, fH / 2.f);
        sprite.setPosition(position);
        sprite.setScale(2.0f, 2.0f); // Dopasuj tę wartość, nowa grafika jest malutka!
    }

    // "Ukrywamy" stary zielony kwadrat, robiąc go przezroczystym reszte zostawiamy
    shape.setSize(sf::Vector2f(30.f, 30.f));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOrigin(15.f, 15.f);
    shape.setPosition(position);
}

void Walker::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    // Odpalamy logikę ruchu z klasy bazowej (chodzenie po ścieżce, obrażenia itp.)
    Zombie::update(dt, objects);

    // --- LOGIKA ANIMACJI ---
    if (areTexturesLoaded && !reachedEnd()) {

        // --- WYBÓR TEKSTURY NA PODSTAWIE KIERUNKU ---
        if (std::abs(velocity.x) > std::abs(velocity.y)) {
            // Ruch poziomy
            if (velocity.x > 0) sprite.setTexture(texRight);
            else sprite.setTexture(texLeft);
        } else {
            // Ruch pionowy
            if (velocity.y > 0) sprite.setTexture(texDown);
            else sprite.setTexture(texUp);
        }

        // --- ANIMACJA  ---
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrame++;

            if (currentFrame >= totalFrames) {
                currentFrame = 0; // Zapętlamy chodzenie
            }

            // Pobieramy rozmiary aktualnie założonej tekstury
            int fW = sprite.getTexture()->getSize().x / totalFrames;
            int fH = sprite.getTexture()->getSize().y;

            // Zawsze rząd 0, bo to pasek
            sprite.setTextureRect(sf::IntRect(currentFrame * fW, 0, fW, fH));
        }
    }
    // Obrazek musi podążać za pozycją zombiaka
    sprite.setPosition(position);
}

void Walker::render(sf::RenderWindow& window) {
    // Jeśli tekstura się wczytała, rysujemy obrazek
    if (areTexturesLoaded) {
        window.draw(sprite);
    }

    Zombie::render(window); // to rysuje przezroczysty kwadrat i pasek HP
}
