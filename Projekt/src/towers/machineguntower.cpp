#include "machineguntower.h"
#include "Bullet.h"
#include "Config.h"
#include "resourcemanager.h"
#include "audiomanager.h"

MachineGunTower::MachineGunTower(sf::Vector2f pos): Tower(pos) {
    cost = Config::MachineGunTower::COST;
    damage = Config::MachineGunTower::DAMAGE;
    range = Config::MachineGunTower::RANGE;
    fireRate = Config::MachineGunTower::FIRE_RATE;
    rotationSpeed = Config::MachineGunTower::ROTATION_SPEED;

    // Wieża ma tylko 2 grafiki — poziom 3 używa tekstury poziomu 2.
    initSprite(&ResourceManager::getTexture(Config::Assets::MG_TOWER_LVL1),
               &ResourceManager::getTexture(Config::Assets::MG_TOWER_LVL2),
               &ResourceManager::getTexture(Config::Assets::MG_TOWER_LVL2),
               0.75f, Config::TOWER_TEXTURE_FORWARD);
}

void MachineGunTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects){
    if (!currentTarget) return;
    AudioManager::playSound(Config::Assets::SOUND_SHOOT, 7.f);
    auto bullet = std::make_unique<Bullet>(position, currentTarget->getPosition(), damage);
    objects.push_back(std::move(bullet));
}
