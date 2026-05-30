#ifndef BULLET_H
#define BULLET_H

#include "Projectile.h"

// Bullet — pocisk pojedynczego celu
// Wystrzeliwany przez MachineGunTower i SniperTower.
// Trafia jednego zombi i zadaje obrażenia DamageType::BULLET
// (redukowane przez Armored o 50%).

class Bullet : public Projectile {
public:
    Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, int damage);

    // Zadaje obrażenia tylko trafionemu zombi, pocisk znika.
    void onHit(Zombie* hitZombie,
               std::vector<std::unique_ptr<GameObject>>& objects) override;
};

#endif // BULLET_H
