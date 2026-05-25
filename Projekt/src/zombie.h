#ifndef ZOMBIE_H
#define ZOMBIE_H

// zombie.h — abstrakcyjna klasa bazowa dla wszystkich typów zombie

#include "Gameobject.h"
#include "Config.h"
#include <vector>

enum class DamageType {
    BULLET,
    EXPLOSION,
    FIRE
};

// Klasa bazowa zombie: porusza się po wyznaczonej ścieżce (waypoints), przyjmuje
// obrażenia różnych typów i może być spowolniona lub podpalona.
class Zombie : public GameObject
{
protected:
    int hp = 0;
    int maxHp = 0;
    int reward = 0;
    float baseSpeed = 0.f;
    float currentSpeed = 0.f;
    const std::vector<sf::Vector2f>& path;
    int currentWaypointIndex = 1;

    // Efekty — przydadzą się w MS3
    float slowTimer = 0.f;
    float burnTimer = 0.f;
    int burnDamage = 0;

    sf::RectangleShape shape;

public:
    Zombie(const std::vector<sf::Vector2f>& path);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;

    // Typ ignorowany w klasie bazowej; klasy pochodne (np. ArmoredZombie) mogą nadpisać.
    virtual void takeDamage(int dmg, DamageType type);

    // Spowalnia zombie na podany czas; nie stackuje się — resetuje timer.
    void applySlow(float duration);

    // Podpala zombie: zadaje dps obrażeń na sekundę przez duration sekund.
    void applyBurn(int dps, float duration);

    int getReward() const { return reward; }
    bool reachedEnd() const;
};

#endif // ZOMBIE_H
