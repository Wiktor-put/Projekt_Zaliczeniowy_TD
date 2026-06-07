#ifndef RUNNER_H
#define RUNNER_H

#include "zombie.h"

// --- Klasa Runner (Szybki, mało HP) ---
// Ruch/animacja/rendering w klasie bazowej Zombie; tu tylko statystyki i grafika.
class Runner : public Zombie
{
public:
    Runner(const std::vector<sf::Vector2f>& waypoints);
};

#endif // RUNNER_H
