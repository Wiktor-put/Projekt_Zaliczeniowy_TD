#include "flamethrowertower.h"
#include "flame.h"
#include "Config.h"

FlamethrowerTower::FlamethrowerTower(sf::Vector2f pos) : Tower(pos) {
    cost = Config::FlamethrowerTower::COST;
    range = Config::FlamethrowerTower::RANGE;
    fireRate = Config::FlamethrowerTower::FIRE_RATE;
    dps = Config::FlamethrowerTower::DAMAGE_PER_SECOND;
    burnDuration = Config::FlamethrowerTower::BURN_DURATION;
    shape.setFillColor(sf::Color(200, 100, 0)); // Pomarańczowa wieża
}

void FlamethrowerTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (currentTarget) {
        objects.push_back(std::make_unique<Flame>(position, currentTarget->getPosition(), dps, burnDuration));
    }
}