#include "runner.h"
#include "Config.h"

Runner::Runner(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    // Statystyki z Config.h
    hp           = Config::Runner::HP;
    maxHp        = Config::Runner::HP;
    baseSpeed    = Config::Runner::SPEED;
    currentSpeed = Config::Runner::SPEED;
    reward       = Config::Runner::REWARD;

    // Czerwony, mniejszy kwadrat
    shape.setSize(sf::Vector2f(25.f, 25.f));
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(12.5f, 12.5f);
    shape.setPosition(position);
}

