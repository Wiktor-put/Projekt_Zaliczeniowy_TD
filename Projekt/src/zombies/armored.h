#ifndef AMORED_H
#define AMORED_H


#include "zombie.h"

// --- Klasa Armored (Wolny, odporny na kule) ---
class Armored : public Zombie
{
public:
    Armored(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy funkcję otrzymywania obrażeń, aby uwzględnić pancerz
    void takeDamage(int dmg, DamageType type) override;
};

#endif //