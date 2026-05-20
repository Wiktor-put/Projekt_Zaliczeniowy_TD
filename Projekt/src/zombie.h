#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Gameobject.h"

enum class DamageType {
    BULLET,
    EXPLOSION,
    FIRE
};

class Zombie: public GameObject
{
protected:
    int hp;
    int maxHp;
    int reward;                              // ile waluty daje przy śmierci
    float baseSpeed;                         // bazowa prędkość (px/s)
    float currentSpeed;                      // aktualna (modyfikowana przez Slower)
    const std::vector<sf::Vector2f>& waypoints;
    int currentWaypointIndex;                // do którego waypointa zmierza

    // Efekty - przydadzą się w MS3
    float slowTimer;
    float burnTimer;
    int burnDamage;

    // Reprezentacja graficzna
    sf::CircleShape shape;
public:
    Zombie(const std::vector<sf::Vector2f>& path);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;

    virtual void takeDamage(int dmg, DamageType type) = 0;
    void applySlow(float duration);
    void applyBurn(int dps, float duration);

    int getReward() const { return reward; }
    bool reachedEnd() const;   // czy zombie dotarł do bazy

};

#endif // ZOMBIE_H
