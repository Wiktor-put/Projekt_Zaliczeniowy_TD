#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Gameobject.h"

class Zombie: public GameObject
{
protected:
    int hp;
    int value;
    double bonusChance;
    int liveCost;
public:
    Zombie();

};

#endif // ZOMBIE_H
