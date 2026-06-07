#include "armored.h"
#include "Config.h"

Armored::Armored(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    hp           = Config::Armored::HP;
    maxHp        = Config::Armored::HP;
    baseSpeed    = Config::Armored::SPEED;
    currentSpeed = Config::Armored::SPEED;
    reward       = Config::Armored::REWARD;
    lifeCost     = Config::Armored::LIFECOST;

    initSprite(Config::Assets::ARMORED_DOWN, Config::Assets::ARMORED_UP,
               Config::Assets::ARMORED_RIGHT, Config::Assets::ARMORED_LEFT,
               8, 0.1f, 2.1f);
}

// Opancerzony zombiak redukuje obrażenia od kul!
void Armored::takeDamage(int dmg, DamageType type) {
    if (type == DamageType::BULLET) {
        // Redukcja obrażeń wg współczynnika z Config (np. 0.5 = 50% mniej).
        float reducedDmg = static_cast<float>(dmg) * (1.0f - Config::Armored::BULLET_RESISTANCE);
        Zombie::takeDamage(static_cast<int>(reducedDmg), type);
    } else {
        // Ogień, wybuch itp. zadają pełne obrażenia.
        Zombie::takeDamage(dmg, type);
    }
}
