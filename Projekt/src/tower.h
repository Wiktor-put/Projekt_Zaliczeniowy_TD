#ifndef TOWER_H
#define TOWER_H

#include "Gameobject.h"

class Tower: public GameObject
{
protected:
    double range;
    int level;
    GameObject* enemy;
    int rateOfFire;
    int demage;
    int cost;

public:
    Tower();
};

#endif // TOWER_H
