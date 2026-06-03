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

        // 3. WYCINANIE: Bierzemy kwadrat zaczynający się w punkcie (X:0, Y:120) o wymiarach 100x120 pikseli.
        sprite.setTextureRect(sf::IntRect(30, 50, 100, 120));

        // 4. Ustawiamy środek obrazka i pozycję
        sprite.setOrigin(50.f, 60.f);
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

    // Obrazek musi podążać za pozycją zombiaka
    sprite.setPosition(position);
}

void Walker::render(sf::RenderWindow& window) {
    // Jeśli tekstura się wczytała, rysujemy obrazek
    if (isTextureLoaded) {
        window.draw(sprite);
    }

    // Wywołujemy render z klasy bazowej (narysuje niewidzialny kwadrat i nasz pasek HP)
    Zombie::render(window);
}