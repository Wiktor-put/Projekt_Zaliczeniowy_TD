#include "zombie.h"
#include <cmath>

Zombie::Zombie(const std::vector<sf::Vector2f>& waypoints)
    : path(waypoints), currentWaypointIndex(1)
{
}

void Zombie::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;  // Zombie nie potrzebuje kontenera w update

    // Obsługa spowolnienia
    if (slowTimer > 0.f) {
        slowTimer -= dt;
        if (slowTimer <= 0.f) {
            currentSpeed = baseSpeed; // Koniec spowolnienia, powrót do normy
        }
    }

    // Obsługa podpalenia (otrzymywanie obrażeń w czasie)
    if (burnTimer > 0.f) {
        burnTimer -= dt;
        burnAccumulator += burnDamage * dt; // Zbieramy ułamkowe obrażenia (np. 15 HP na sekundę)

        if (burnAccumulator >= 1.f) {
            int dmgToTake = static_cast<int>(burnAccumulator);
            takeDamage(dmgToTake, DamageType::FIRE); // Odbieramy punkty życia
            burnAccumulator -= dmgToTake;
        }
    }

    if (!reachedEnd()) {
        sf::Vector2f target = path[currentWaypointIndex];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 2.f) {
            sf::Vector2f normalizedDir = direction / distance;
            velocity = normalizedDir * currentSpeed;
            position += velocity * dt;
        } else {
            currentWaypointIndex++;
        }
    }
    else destroy();

    shape.setPosition(position);
}

void Zombie::render(sf::RenderWindow& window) {
    window.draw(shape);

    // Pasek HP
    float ratio = (maxHp > 0) ? static_cast<float>(hp) / static_cast<float>(maxHp) : 0.f;
    sf::RectangleShape bgBar(sf::Vector2f(30.f, 4.f));
    bgBar.setFillColor(sf::Color::Red);
    bgBar.setPosition(position - sf::Vector2f(15.f, 20.f));
    window.draw(bgBar);

    sf::RectangleShape hpBar(sf::Vector2f(30.f * ratio, 4.f));
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setPosition(position - sf::Vector2f(15.f, 20.f));
    window.draw(hpBar);
}

void Zombie::takeDamage(int dmg, DamageType type) {
    (void)type;
    hp -= dmg;
    if (hp <= 0)
        destroy();
}

void Zombie::applySlow(float duration) {
    slowTimer = duration;
    currentSpeed = baseSpeed * 0.5f;
}

void Zombie::applyBurn(int dps, float duration) {
    burnTimer = duration;
    burnDamage = dps;
}

bool Zombie::reachedEnd() const {
    return currentWaypointIndex >= static_cast<int>(path.size());
}
