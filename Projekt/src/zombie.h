#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Gameobject.h"
#include "Config.h" // Konieczne do pobrania statystyk

class Zombie : public GameObject
{
protected:
    int hp;
    int value; // nagroda (REWARD)
    double bonusChance;
    int liveCost;
    float speed; // Zmienna na bazową prędkość zombie

    // Kształt do testowego wyświetlania (zamiast tekstury)
    sf::RectangleShape shape;

public:
    Zombie();

    // Wspólna logika dla wszystkich zombie (ruch po ekranie)
    virtual void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    // Wspólne rysowanie
    virtual void render(sf::RenderWindow& window) override;
};

//zwykły zombiak
class Walker : public Zombie
{
public:
    // Konstruktor przyjmujący pozycję startową
    Walker(sf::Vector2f startPosition);
};


#endif // ZOMBIE_H