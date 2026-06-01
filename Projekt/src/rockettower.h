#ifndef ROCKETTOWER_H
#define ROCKETTOWER_H

#include "tower.h"

class RocketTower : public Tower {
private:
    float aoeRadius;

public:
    RocketTower(sf::Vector2f pos);
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};

#endif // ROCKETTOWER_H