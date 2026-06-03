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

    // Szary, nieco większy kwadrat
    shape.setSize(sf::Vector2f(35.f, 35.f));
    shape.setFillColor(sf::Color(128, 128, 128));
    shape.setOrigin(17.5f, 17.5f);
    shape.setPosition(position);
}

// Opancerzony zombiak redukuje obrażenia od kul!
void Armored::takeDamage(int dmg, DamageType type) {
    if (type == DamageType::BULLET) {
        // Redukcja obrażeń (np. 0.5f, czyli 50% mniej)
        float reducedDmg = static_cast<float>(dmg) * (1.0f - Config::Armored::BULLET_RESISTANCE);

        // Wywołanie oryginalnej funkcji z bazowej klasy, ale z mniejszymi obrażeniami
        Zombie::takeDamage(static_cast<int>(reducedDmg), type);
    } else {
        // Inne obrażenia (np. ogień z miotacza lub wybuch) zadają pełne obrażenia
        Zombie::takeDamage(dmg, type);
    }
}