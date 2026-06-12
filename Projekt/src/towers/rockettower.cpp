#include "rockettower.h"
#include "rocket.h"
#include "Config.h"
#include "resourcemanager.h"
#include "audiomanager.h"

RocketTower::RocketTower(sf::Vector2f pos) : Tower(pos) {
    // Pobranie statystyk dla wyrzutni rakiet
    cost = Config::RocketTower::COST;
    damage = Config::RocketTower::DAMAGE;
    range = Config::RocketTower::RANGE;
    fireRate = Config::RocketTower::FIRE_RATE;
    rotationSpeed = Config::RocketTower::ROTATION_SPEED;
    aoeRadius = Config::RocketTower::AOE_RADIUS;

    // Wyrzutnia ma 3 osobne grafiki poziomów.
    initSprite(&ResourceManager::getTexture(Config::Assets::ROCKET_TOWER_LVL1),
               &ResourceManager::getTexture(Config::Assets::ROCKET_TOWER_LVL2),
               &ResourceManager::getTexture(Config::Assets::ROCKET_TOWER_LVL3),
               0.75f, Config::TOWER_TEXTURE_FORWARD);
}

void RocketTower::shoot(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (!currentTarget) return;

    AudioManager::playSound(Config::Assets::SOUND_EXPLOSION, 15.f);

    // Zamiast standardowego "Bullet", tworzymy naszą Rakietę!
    auto rocket = std::make_unique<Rocket>(position, currentTarget->getPosition(), damage, aoeRadius);
    objects.push_back(std::move(rocket));
}