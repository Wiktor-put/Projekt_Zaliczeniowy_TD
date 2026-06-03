#include "rockettower.h"
#include "rocket.h"
#include "Config.h"

RocketTower::RocketTower(sf::Vector2f pos) : Tower(pos) {
    // Pobranie statystyk dla wyrzutni rakiet
    cost = Config::RocketTower::COST;
    damage = Config::RocketTower::DAMAGE;
    range = Config::RocketTower::RANGE;
    fireRate = Config::RocketTower::FIRE_RATE;
    rotationSpeed = Config::RocketTower::ROTATION_SPEED;
    aoeRadius = Config::RocketTower::AOE_RADIUS;

    // Ciemnoczerwony kolor dla wyrzutni
    shape.setFillColor(sf::Color(150, 50, 50));
}

void RocketTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (!currentTarget) return;

    // Zamiast standardowego "Bullet", tworzymy naszą Rakietę!
    auto rocket = std::make_unique<Rocket>(position, currentTarget->getPosition(), damage, aoeRadius);
    objects.push_back(std::move(rocket));
}