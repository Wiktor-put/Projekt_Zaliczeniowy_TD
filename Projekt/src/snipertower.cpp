#include "snipertower.h"
#include "bullet.h" // Snajper też strzela zwykłymi kulami
#include "Config.h"

SniperTower::SniperTower(sf::Vector2f pos) : Tower(pos) {
    cost = Config::SniperTower::COST;
    damage = Config::SniperTower::DAMAGE;
    range = Config::SniperTower::RANGE;
    fireRate = Config::SniperTower::FIRE_RATE;
    rotationSpeed = Config::SniperTower::ROTATION_SPEED;

    // Ciemnozielony kolor dla snajpera
    shape.setFillColor(sf::Color(30, 100, 30));
}

void SniperTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (!currentTarget) return;

    // Tworzymy pocisk i celujemy w zombiaka, dokładnie jak w MachineGunTower
    auto bullet = std::make_unique<Bullet>(position, currentTarget->getPosition(), damage);
    objects.push_back(std::move(bullet));
}