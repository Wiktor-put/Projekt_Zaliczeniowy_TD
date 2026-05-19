#include "machineguntower.h"

MachineGunTower::MachineGunTower(sf::Vector2f pos): Tower(pos) {
    cost = Config::MachineGunTower::COST;
    damage = Config::MachineGunTower::DAMAGE;
    range = Config::MachineGunTower::RANGE;
    fireRate = Config::MachineGunTower::FIRE_RATE;
    rotationSpeed = Config::MachineGunTower::ROTATION_SPEED;
}

void MachineGunTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects){

}
