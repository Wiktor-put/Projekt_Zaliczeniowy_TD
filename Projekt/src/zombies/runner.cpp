#include "runner.h"
#include "Config.h"

Runner::Runner(const std::vector<sf::Vector2f>& waypoints) : Zombie(waypoints) {
    position = waypoints[0];
    alive = true;

    hp           = Config::Runner::HP;
    maxHp        = Config::Runner::HP;
    baseSpeed    = Config::Runner::SPEED;
    currentSpeed = Config::Runner::SPEED;
    reward       = Config::Runner::REWARD;
    lifeCost     = Config::Runner::LIFECOST;

    // 8 klatek, szybka animacja (0.06s) — efekt biegu.
    initSprite(Config::Assets::RUNNER_DOWN, Config::Assets::RUNNER_UP,
               Config::Assets::RUNNER_RIGHT, Config::Assets::RUNNER_LEFT,
               8, 0.06f, 1.9f);
}
