#ifndef ROCKET_H
#define ROCKET_H

#include "projectile.h"

// Rakieta - leci do celu, a przy trafieniu wybucha zadając obrażenia obszarowe
class Rocket : public Projectile {
private:
    float aoeRadius;    // promień obszarowych obrażeń wybuchu (px)
    sf::Sprite sprite;  // tekstura rakiety (obracana zgodnie z kierunkiem lotu)

public:
    // Konstruktor przyjmuje dodatkowo promień wybuchu
    Rocket(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float aoeRadius);

    void onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) override;

    // Nadpisujemy rendering, by narysować obróconą teksturę zamiast kółka.
    void render(sf::RenderWindow& window) override;

    // Zadaje obrażenia obszarowe wszystkim zombie w promieniu aoeRadius od miejsca wybuchu.
    void explosion(std::vector<std::unique_ptr<GameObject>>& objects);
};

#endif // ROCKET_H
