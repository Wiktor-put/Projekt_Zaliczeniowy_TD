#ifndef MACHINEGUNTOWER_H
#define MACHINEGUNTOWER_H

#include "tower.h"
#include "Config.h"

class MachineGunTower: public Tower
{
public:
    MachineGunTower(sf::Vector2f pos);
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};

#endif // MACHINEGUNTOWER_H
