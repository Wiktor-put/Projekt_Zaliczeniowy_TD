#ifndef ROCKET_H
#define ROCKET_H

#include "projectile.h"

// Rakieta - leci do celu, a przy trafieniu wybucha zadając obrażenia obszarowe
class Rocket : public Projectile {
private:
    float aoeRadius;
    sf::Sprite sprite;  // tekstura rakiety (obracana zgodnie z kierunkiem lotu)

public:
    // Konstruktor przyjmuje dodatkowo promień wybuchu
    Rocket(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float aoeRadius);

    void onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) override;

    // Nadpisujemy rendering, by narysować obróconą teksturę zamiast kółka.
    void render(sf::RenderWindow& window) override;

    void explosion(std::vector<std::unique_ptr<GameObject>>& objects);
};

#endif // ROCKET_H
