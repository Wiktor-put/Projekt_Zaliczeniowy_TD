/*#ifndef
#define

#include "zombie.h"
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


#endif //
*/