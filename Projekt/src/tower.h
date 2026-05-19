#ifndef TOWER_H
#define TOWER_H

#include <memory>
#include <cmath>
#include "Gameobject.h"
#include "zombie.h"

class Tower: public GameObject
{
protected:
    float range;
    float fireRate;
    float cooldown;
    int damage;
    int cost;
    int level;
    Zombie* currentTarget;

public:
    Tower(sf::Vector2f pos);
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    virtual void shoot(std::vector<std::unique_ptr<GameObject>>& objects);
    void render(sf::RenderWindow& window) override;
    void findTarget(const std::vector<std::unique_ptr<GameObject>>& objects);
    bool isTargetValid() const;
    void upgrade();
    void rotateToward(sf::Vector2f targetPos, float dt);
};

#endif // TOWER_H
