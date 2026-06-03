#ifndef RUNNER_H
#define RUNNER_H

#include "zombie.h"
// --- Klasa Runner (Szybki, mało HP) ---
class Runner : public Zombie
{
public:
    Runner(const std::vector<sf::Vector2f>& waypoints);
};

#endif //
