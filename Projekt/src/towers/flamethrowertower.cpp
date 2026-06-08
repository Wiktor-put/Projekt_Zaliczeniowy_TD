#include "flamethrowertower.h"
#include "flame.h"
#include "Config.h"
#include "resourcemanager.h"

FlamethrowerTower::FlamethrowerTower(sf::Vector2f pos) : Tower(pos) {
    cost = Config::FlamethrowerTower::COST;
    range = Config::FlamethrowerTower::RANGE;
    fireRate = Config::FlamethrowerTower::FIRE_RATE;
    dps = Config::FlamethrowerTower::DAMAGE_PER_SECOND;
    burnDuration = Config::FlamethrowerTower::BURN_DURATION;
    rotationSpeed =Config::FlamethrowerTower::ROTATION_SPEED;

    initSprite(&ResourceManager::getTexture(Config::Assets::FLAME_TOWER_LVL1),
               &ResourceManager::getTexture(Config::Assets::FLAME_TOWER_LVL2),
               &ResourceManager::getTexture(Config::Assets::FLAME_TOWER_LVL3),
               0.75f, Config::TOWER_TEXTURE_FORWARD);
}

void FlamethrowerTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (currentTarget) {
        objects.push_back(std::make_unique<Flame>(position, currentTarget->getPosition(), dps, burnDuration));
    }
}