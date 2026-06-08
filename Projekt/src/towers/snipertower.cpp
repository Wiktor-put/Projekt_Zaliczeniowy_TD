#include "snipertower.h"
#include "bullet.h" // Snajper też strzela zwykłymi kulami
#include "resourcemanager.h"
#include "Config.h"

SniperTower::SniperTower(sf::Vector2f pos) : Tower(pos) {
    cost = Config::SniperTower::COST;
    damage = Config::SniperTower::DAMAGE;
    range = Config::SniperTower::RANGE;
    fireRate = Config::SniperTower::FIRE_RATE;
    rotationSpeed = Config::SniperTower::ROTATION_SPEED;

    initSprite(&ResourceManager::getTexture(Config::Assets::SNIPER_TOWER),
               &ResourceManager::getTexture(Config::Assets::SNIPER_TOWER),
               &ResourceManager::getTexture(Config::Assets::SNIPER_TOWER),
               0.75f, Config::TOWER_TEXTURE_FORWARD);
}

void SniperTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (!currentTarget) return;

    // Tworzymy pocisk i celujemy w zombiaka, dokładnie jak w MachineGunTower
    auto bullet = std::make_unique<Bullet>(position, currentTarget->getPosition(), damage);
    objects.push_back(std::move(bullet));
}

void SniperTower::upgrade() {
    Tower::upgrade(); // Zwiększa damage i zasięg

    // Zwiększa rozmiar snajperki wizualnie!
    float currentScale = sprite.getScale().x;
    sprite.setScale(currentScale * 1.3f, currentScale * 1.3f);
}