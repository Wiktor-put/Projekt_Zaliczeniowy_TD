#include "Projectile.h"
#include "Config.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f targetPos, int damage)
    : damage(damage), target(targetPos), speed(0.f)
{
    position = startPos;

    // Wektor jednostkowy w stronę celu - klasa pochodna pomnoży przez speed
    // w swoim konstruktorze (bo dopiero ona zna swoją prędkość z Config).
    sf::Vector2f diff = target - position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (dist > 0.f) {
        velocity = diff / dist;  // długość 1, kierunek do celu
    }

    rotation = std::atan2(diff.y, diff.x) * 180.f / 3.14159f;
}

void Projectile::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;  // pocisk nie potrzebuje kontenera - kolizje obsługuje Game

    // Ruch po linii prostej
    position += velocity * dt;
    shape.setPosition(position);

    // Bezpiecznik: pocisk wyleciał poza okno (z marginesem 50px)
    if (position.x < -50.f || position.x > Config::WINDOW_WIDTH + 50.f ||
        position.y < -50.f || position.y > Config::WINDOW_HEIGHT + 50.f) {
        destroy();
        return;
    }

}

bool Projectile::hasReachedTarget() const {
    sf::Vector2f diff = target - position;
    return (diff.x * diff.x + diff.y * diff.y) < 225.f;  // 15px tolerancji
}

void Projectile::render(sf::RenderWindow& window) {
    window.draw(shape);
}

