#ifndef SNOWBALL_H
#define SNOWBALL_H
#include "projectile.h"

// Snowball — pocisk wieży spowalniającej. Nie zadaje obrażeń, lecz po trafieniu
// nakłada na zombie spowolnienie na zadany czas.
class Snowball : public Projectile {
private:
    float duration;  // czas trwania spowolnienia nakładanego na cel (s)
public:
    Snowball(sf::Vector2f startPos, sf::Vector2f targetPos, float duration);
    // Nakłada na trafione zombie efekt spowolnienia (applySlow) i niszczy pocisk.
    void onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif