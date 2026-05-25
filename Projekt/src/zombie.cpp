#include "zombie.h"
#include "Config.h"
#include <cmath>


//   KLASA BAZOWA ZOMBIE
Zombie::Zombie(const std::vector<sf::Vector2f>& waypoints) {
    path = waypoints;
    currentWaypointIndex = 1; // Zaczynamy wędrówkę do punktu 1 (na punkcie 0 się spawnujemy)
}

void Zombie::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    if (currentWaypointIndex < path.size()) {

        sf::Vector2f target = path[currentWaypointIndex];
        sf::Vector2f direction = target - position;

        // Obliczamy dystans (twierdzenie Pitagorasa)
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 2.f) {
            // Idziemy w stronę celu
            sf::Vector2f normalizedDir = direction / distance;
            position += normalizedDir * speed * dt;
        } else {
            // Jesteśmy blisko punktu, przełączamy cel na następny
            currentWaypointIndex++;
        }
    } else {
        // Zombie dotarł do końca mapy!
        destroy();
    }

    shape.setPosition(position);
}

void Zombie::render(sf::RenderWindow& window) {
    window.draw(shape);
}

void Zombie::takeDamage(int dmg, DamageType type) {
    (void)type;  // typ ignorowany w klasie bazowej, Armored nadpisze
    hp -= dmg;
    if (hp <= 0) {
        destroy();  // ustawia alive = false
    }
}

void Zombie::applySlow(float duration) {
    slowTimer = duration;
    currentSpeed = baseSpeed * 0.5f;  // 50% prędkości - w MS3 wartość z Config
}

void Zombie::applyBurn(int dps, float duration) {
    burnTimer = duration;
    burnDamage = dps;
}

bool Zombie::reachedEnd() const {
    return currentWaypointIndex >= static_cast<int>(waypoints.size());
}

Walker::Walker(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    // Ustawienie punktu startowego
    position = waypoints[0];
    alive = true;

    // Przypisanie statystyk z pliku Config.h
    hp = Config::Walker::HP;
    speed = Config::Walker::SPEED;
    value = Config::Walker::REWARD;

    // Ustawienie wyglądu
    shape.setSize(sf::Vector2f(30.f, 30.f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(15.f, 15.f);
    shape.setPosition(position);
}
