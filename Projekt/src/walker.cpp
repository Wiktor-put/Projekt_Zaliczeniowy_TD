#include "walker.h"
#include "Config.h"

Walker::Walker(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints)
{
    position = waypoints[0];
    alive = true;

    hp       = Config::Walker::HP;
    maxHp    = Config::Walker::HP;
    baseSpeed    = Config::Walker::SPEED;
    currentSpeed = Config::Walker::SPEED;
    reward   = Config::Walker::REWARD;

    shape.setSize(sf::Vector2f(30.f, 30.f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(15.f, 15.f);
    shape.setPosition(position);
}
