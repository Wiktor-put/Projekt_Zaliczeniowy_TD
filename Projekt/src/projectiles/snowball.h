#ifndef SNOWBALL_H
#define SNOWBALL_H
#include "projectile.h"

class Snowball : public Projectile {
private:
    float duration;
public:
    Snowball(sf::Vector2f startPos, sf::Vector2f targetPos, float duration);
    void onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif