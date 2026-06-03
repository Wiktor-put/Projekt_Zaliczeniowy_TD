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

    // Fioletowy (Magenta), wielki kwadrat reprezentujący bossa
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setFillColor(sf::Color::Magenta);
    shape.setOrigin(25.f, 25.f);
    shape.setPosition(position);
}
