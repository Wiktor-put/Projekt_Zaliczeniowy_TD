#ifndef FLAME_H
#define FLAME_H
#include "projectile.h"

class Flame : public Projectile {
private:
    int dps;
    float duration;
public:
    Flame(sf::Vector2f startPos, sf::Vector2f targetPos, int dps, float duration);
    void onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif