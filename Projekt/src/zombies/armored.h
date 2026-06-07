#ifndef AMORED_H
#define AMORED_H

#include "zombie.h"

// --- Klasa Armored (Wolny, odporny na kule) ---
// Ruch/animacja/rendering w klasie bazowej; nadpisuje tylko takeDamage (pancerz).
class Armored : public Zombie
{
public:
    Armored(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy otrzymywanie obrażeń, aby uwzględnić odporność na kule.
    void takeDamage(int dmg, DamageType type) override;
};

#endif // AMORED_H
