#include "walker.h"
#include "Config.h"

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
        if (texture.loadFromFile(std::string(ASSETS_DIR) + "/assets/textures/zombies.jpg")) {
            isTextureLoaded = true;
        } else {
            std::cerr << "Nie udalo sie zaladowac grafiki zombies.jpg!" << std::endl;
        }
    }

    // 2. Podpinamy wczytaną teksturę do naszego sprite'a
    if (isTextureLoaded) {
        sprite.setTexture(texture);

        // 2. Konfiguracja animacji z wyliczeniem rozmiarów
        frameWidth = texture.getSize().x / 8;
        frameHeight = texture.getSize().y / 4;

        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);


        sprite.setPosition(position);
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
        animationTimer += dt;

        // Jeśli minął ułamek sekundy, przeskakujemy na kolejną klatkę
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;   // Resetujemy stoper
            currentFrame++;         // Następna klatka

            // Jeśli doszliśmy do końca rzędu, zapętlamy od zera
            if (currentFrame >= totalFrames) {
                currentFrame = 0;
            }

            // Przesuwamy "okienko" wycinania w prawo o (currentFrame * frameWidth)
            // Używamy y = 0, czyli bierzemy pierwszy, górny rząd z obrazka
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        }

        // --- SKALOWANIE I ODBICIE LUSTRZANE ---
        // Skalowanie rozmiaru zombie
        float scaleFactor = 0.4f;

        if (velocity.x < -0.1f) {
            // Zombiak idzie w lewo -> odwracamy oś X (ujemna skala)
            sprite.setScale(-scaleFactor, scaleFactor);
        } else if (velocity.x > 0.1f) {
            // Zombiak idzie w prawo -> normalna skala
            sprite.setScale(scaleFactor, scaleFactor);
        } else {
            // Jeśli idzie pionowo, zachowujemy ostatni kierunek X, ale aplikujemy nowy rozmiar
            float currentSign = (sprite.getScale().x < 0) ? -1.f : 1.f;
            sprite.setScale(currentSign * scaleFactor, scaleFactor);
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