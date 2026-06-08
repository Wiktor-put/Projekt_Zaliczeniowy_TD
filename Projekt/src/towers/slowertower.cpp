#include "slowertower.h"
#include "snowball.h"
#include "resourcemanager.h"
#include "Config.h"

SlowerTower::SlowerTower(sf::Vector2f pos) : Tower(pos) {
    cost = Config::SlowerTower::COST;
    damage = 5; // Minimalne obrażenia fizyczne
    range = Config::SlowerTower::RANGE;
    fireRate = Config::SlowerTower::FIRE_RATE;
    slowDuration = Config::SlowerTower::SLOW_DURATION;
    rotationSpeed = Config::SlowerTower::ROTATION_SPEED;

    initSprite(&ResourceManager::getTexture(Config::Assets::SLOWER_TOWER_LVL1),
               &ResourceManager::getTexture(Config::Assets::SLOWER_TOWER_LVL2),
               &ResourceManager::getTexture(Config::Assets::SLOWER_TOWER_LVL3),
               0.75f, Config::TOWER_TEXTURE_FORWARD);
}

void SlowerTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (currentTarget) {
        objects.push_back(std::make_unique<Snowball>(position, currentTarget->getPosition(), slowDuration));


    }
}