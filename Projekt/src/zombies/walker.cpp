#include "walker.h"
#include "Config.h"
#include <cmath>

// Inicjalizacja statycznych zmiennych klasy
sf::Texture Walker::texture;
bool Walker::isTextureLoaded = false;

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
    if (!isTextureLoaded) {
        if (texture.loadFromFile(std::string(ASSETS_DIR) + "/assets/textures/walker.png")) {
            isTextureLoaded = true;
        } else {
            std::cerr << "Nie udalo sie zaladowac grafiki walker.png!" << std::endl;
        }
    }

    // 2. Podpinamy wczytaną teksturę do naszego sprite'a
    if (isTextureLoaded) {
        sprite.setTexture(texture);

        // 2. Konfiguracja animacji z wyliczeniem rozmiarów
        frameWidth = texture.getSize().x / 8;
        frameHeight = texture.getSize().y / 6;

        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);

        sprite.setPosition(position);

        sprite.setScale(0.3f, 0.3f);
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
    if (isTextureLoaded && !reachedEnd()) {

        // --- WYBÓR KIERUNKU (RZĘDU) ---
        // Sprawdzamy, czy prędkość jest większa na osi X czy Y
        if (std::abs(velocity.x) > std::abs(velocity.y)) {
            // Ruch poziomy
            if (velocity.x > 0) currentRow = 2;      // Prawo (Rząd 3 na obrazku)
            else currentRow = 3;                     // Lewo (Rząd 4 na obrazku)
        } else {
            // Ruch pionowy
            if (velocity.y > 0) currentRow = 0;      // Dół (Rząd 1 na obrazku)
            else currentRow = 1;                     // Góra (Rząd 2 na obrazku)
        }

        // --- ANIMACJA (ZMIANA KLATEK) ---
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrame++;

            if (currentFrame >= totalFrames) {
                currentFrame = 0; // Zapętlamy chodzenie
            }

            // Przesuwamy okienko wycinania w oparciu o wyliczony rząd i klatkę
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight));
        }
    }
    // Obrazek musi podążać za pozycją zombiaka
    sprite.setPosition(position);
}

void Walker::render(sf::RenderWindow& window) {
    // Jeśli tekstura się wczytała, rysujemy obrazek
    if (isTextureLoaded) {
        window.draw(sprite);
    }

    Zombie::render(window); // to rysuje przezroczysty kwadrat i pasek HP
}