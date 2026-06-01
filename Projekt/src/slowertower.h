#ifndef SLOWERTOWER_H
#define SLOWERTOWER_H
#include "tower.h"

class SlowerTower : public Tower {
private:
    float slowDuration;
public:
    SlowerTower(sf::Vector2f pos);
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif