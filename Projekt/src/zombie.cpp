#include "Zombie.h"
#include <cmath>

Zombie::Zombie(const std::vector<sf::Vector2f>& path)
    : waypoints(path),
    hp(100), maxHp(100), reward(10),
    baseSpeed(50.f), currentSpeed(50.f),
    currentWaypointIndex(0),
    slowTimer(0.f), burnTimer(0.f), burnDamage(0)
{
    // Pozycja startowa - pierwszy waypoint
    if (!waypoints.empty()) {
        position = waypoints[0];
        currentWaypointIndex = 1;  // celuj w drugi waypoint
    }

    // Placeholder grafika - kółko o promieniu 12
    shape.setRadius(12.f);
    shape.setOrigin(12.f, 12.f);
    shape.setFillColor(sf::Color::Green);
}

void Zombie::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;  // nieużywane w Zombie - ignore parameter warning

    // Tickanie efektu spowolnienia
    if (slowTimer > 0.f) {
        slowTimer -= dt;
        if (slowTimer <= 0.f) {
            currentSpeed = baseSpeed;  // powrót do normalnej prędkości
        }
    }

    // Tickanie efektu podpalenia
    if (burnTimer > 0.f) {
        burnTimer -= dt;
        takeDamage(static_cast<int>(burnDamage * dt), DamageType::FIRE);
    }

    // Czy są jeszcze waypointy?
    if (currentWaypointIndex >= static_cast<int>(waypoints.size())) {
        return;  // dotarłem do końca
    }

    // Wektor do następnego waypointa
    sf::Vector2f target = waypoints[currentWaypointIndex];
    sf::Vector2f diff = target - position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // Czy dotarłem do waypointa?
    if (dist < 5.f) {  // 5px tolerancji
        position = target;
        currentWaypointIndex++;
        return;
    }

    // Normalizacja - kierunek ruchu (jednostkowy wektor)
    sf::Vector2f direction = diff / dist;

    // Aktualizacja prędkości (do polimorficznej animacji w przyszłości)
    velocity = direction * currentSpeed;

    // Ruch o velocity * dt
    position += velocity * dt;

    // Obrót w kierunku ruchu (do wizualizacji, w MS5 sprite)
    rotation = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;

    // Aktualizacja pozycji shape
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
