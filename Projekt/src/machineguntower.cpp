#include "machineguntower.h"
#include "Bullet.h"
#include "Config.h"

MachineGunTower::MachineGunTower(sf::Vector2f pos): Tower(pos) {
    cost = Config::MachineGunTower::COST;
    damage = Config::MachineGunTower::DAMAGE;
    range = Config::MachineGunTower::RANGE;
    fireRate = Config::MachineGunTower::FIRE_RATE;
    rotationSpeed = Config::MachineGunTower::ROTATION_SPEED;

    shape.setFillColor(sf::Color(100, 100, 130));
}

void MachineGunTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects){
    if (!currentTarget) return;
    auto bullet = std::make_unique<Bullet>(position, currentTarget->getPosition(), damage);
    objects.push_back(std::move(bullet));
}
