#ifndef TANK_H
#define TANK_H

#include "zombie.h"

// --- Klasa Tank (Bardzo wolny, ogromna ilość HP) ---
class Tank : public Zombie
{
public:
    Tank(const std::vector<sf::Vector2f>& waypoints);
};


#endif //