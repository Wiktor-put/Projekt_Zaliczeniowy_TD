#include "walker.h"
#include "Config.h"

Walker::Walker(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints)
{
    position = waypoints[0];
    alive = true;

    hp           = Config::Walker::HP;
    maxHp        = Config::Walker::HP;
    baseSpeed    = Config::Walker::SPEED;
    currentSpeed = Config::Walker::SPEED;
    reward       = Config::Walker::REWARD;
    lifeCost     = Config::Walker::LIFECOST;

    // Tekstury, liczba klatek (6), tempo animacji i skala. Reszta dzieje się w bazie.
    initSprite(Config::Assets::WALKER_DOWN, Config::Assets::WALKER_UP,
               Config::Assets::WALKER_RIGHT, Config::Assets::WALKER_LEFT,
               6, 0.1f, 1.8f);
}
