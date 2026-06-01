#ifndef WALKER_H
#define WALKER_H

// walker.h — podstawowy typ zombie poruszający się po ścieżce

#include "zombie.h"

// Najsłabszy wariant zombie: niskie HP, niska prędkość, mała nagroda.
// Statystyki wczytywane z Config::Walker.
class Walker : public Zombie
{
public:
    Walker(const std::vector<sf::Vector2f>& waypoints);
};


// --- Klasa Runner (Szybki, mało HP) ---
class Runner : public Zombie
{
public:
    Runner(const std::vector<sf::Vector2f>& waypoints);
};

// --- Klasa Armored (Wolny, odporny na kule) ---
class Armored : public Zombie
{
public:
    Armored(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy funkcję otrzymywania obrażeń, aby uwzględnić pancerz
    void takeDamage(int dmg, DamageType type) override;
};

// --- Klasa Tank (Bardzo wolny, ogromna ilość HP) ---
class Tank : public Zombie
{
public:
    Tank(const std::vector<sf::Vector2f>& waypoints);
};



#endif // WALKER_H
