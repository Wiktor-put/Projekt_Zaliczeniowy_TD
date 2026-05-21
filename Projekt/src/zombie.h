#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Gameobject.h"
#include "Config.h"
#include <vector>

// === KLASA BAZOWA ===
class Zombie : public GameObject
{
protected:
    int hp;
    int value; // nagroda
    double bonusChance;
    int liveCost;
    float speed; // Zmienna na bazową prędkość zombie
    sf::RectangleShape shape; // Kształt do testowego wyświetlania (zamiast tekstury)
    // Zmienne do nawigacji
    std::vector<sf::Vector2f> path;
    size_t currentWaypointIndex;


public:
    Zombie(const std::vector<sf::Vector2f>& waypoints);

    // Wspólna logika dla wszystkich zombie (ruch po ekranie)
    virtual void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;

    virtual void render(sf::RenderWindow& window) override; // Wspólne rysowanie
};

//=== WALKER ===
class Walker : public Zombie
{
public:
    // Konstruktor przyjmujący pozycję startową
    //Walker(sf::Vector2f startPosition);
    // droga
    Walker(const std::vector<sf::Vector2f>& waypoints);
};


#endif // ZOMBIE_H