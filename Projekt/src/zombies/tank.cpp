#include "tank.h"
#include "Config.h"

Tank::Tank(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    hp           = Config::Tank::HP;
    maxHp        = Config::Tank::HP;
    baseSpeed    = Config::Tank::SPEED;
    currentSpeed = Config::Tank::SPEED;
    reward       = Config::Tank::REWARD;
    lifeCost     = Config::Tank::LIFECOST;

    // 8 klatek, ociężała animacja (0.15s), duża skala bossa (3.1).
    initSprite(Config::Assets::TANK_DOWN, Config::Assets::TANK_UP,
               Config::Assets::TANK_RIGHT, Config::Assets::TANK_LEFT,
               8, 0.15f, 3.1f);

    // Korekta wizualna: duży model rysujemy nieco wyżej, by stopy były na ścieżce.
    spriteOffset.y = -20.f;
}
