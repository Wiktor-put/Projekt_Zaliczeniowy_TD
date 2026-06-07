#ifndef WALKER_H
#define WALKER_H

// walker.h — podstawowy typ zombie poruszający się po ścieżce

#include "zombie.h"

// Najsłabszy wariant zombie: niskie HP, niska prędkość, mała nagroda.
// Statystyki wczytywane z Config::Walker. Ruch, animacja i rendering są
// w pełni obsługiwane przez klasę bazową Zombie — tu tylko konfiguracja.
class Walker : public Zombie
{
public:
    Walker(const std::vector<sf::Vector2f>& waypoints);
};

#endif // WALKER_H
