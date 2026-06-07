#ifndef TANK_H
#define TANK_H

#include "zombie.h"

// --- Klasa Tank (Bardzo wolny, ogromna ilość HP) ---
// Ruch/animacja/rendering w klasie bazowej Zombie; tu tylko statystyki i grafika.
class Tank : public Zombie
{
public:
    Tank(const std::vector<sf::Vector2f>& waypoints);
};

#endif // TANK_H
