#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H

#include "tower.h"

// Wieża o ogromnym zasięgu i obrażeniach, ale bardzo wolnym strzelaniu.
class SniperTower : public Tower
{
public:
    SniperTower(sf::Vector2f pos);
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};

#endif // SNIPERTOWER_H