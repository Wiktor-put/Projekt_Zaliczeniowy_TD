#include "slowertower.h"
#include "snowball.h"
#include "Config.h"

SlowerTower::SlowerTower(sf::Vector2f pos) : Tower(pos) {
    cost = Config::SlowerTower::COST;
    range = Config::SlowerTower::RANGE;
    fireRate = Config::SlowerTower::FIRE_RATE;
    slowDuration = Config::SlowerTower::SLOW_DURATION;
    shape.setFillColor(sf::Color(100, 200, 255)); // Jasnoniebieska wieża
}

void SlowerTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (currentTarget) {
        objects.push_back(std::make_unique<Snowball>(position, currentTarget->getPosition(), slowDuration));
    }
}