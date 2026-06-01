#ifndef FLAMETHROWERTOWER_H
#define FLAMETHROWERTOWER_H
#include "tower.h"

class FlamethrowerTower : public Tower {
private:
    int dps;
    float burnDuration;
public:
    FlamethrowerTower(sf::Vector2f pos);
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif