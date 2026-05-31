#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Gameobject.h"
#include "Zombie.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

// Projectile.h — abstrakcyjna baza dla pocisków
// Wspólna logika: lot po linii prostej w stronę zapamiętanej pozycji celu.
// Pocisk znika gdy: trafi zombi (kolizja w Game), dotrze do celu, wyleci poza ekran.
// Klasy pochodne (Bullet, Rocket) implementują onHit() z własną mechaniką trafienia.

class Projectile : public GameObject
{
protected:
    int damage;
    sf::Vector2f target;    // pozycja celu w momencie wystrzału (pocisk nie naprowadza)
    float speed;            // prędkość lotu (px/s) - ustawia klasa pochodna

    sf::CircleShape shape;

public:
    Projectile(sf::Vector2f startPos, sf::Vector2f targetPos, int damage);

    // Wspólna logika lotu i bezpiecznika poza ekranem.
    // Klasy pochodne nie nadpisują - mają tę samą fizykę ruchu.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;

    // Wywoływane przez Game::checkCollisions przy zderzeniu z zombi.
    // Bullet: pojedynczy cel. Rocket: AoE w promieniu.
    virtual void onHit(Zombie* hitZombie,
                       std::vector<std::unique_ptr<GameObject>>& objects) = 0;

    int getDamage() const { return damage; }

    // Zwraca true jeśli pocisk dotarł do zapamiętanej pozycji celu (tolerancja 15px).
    // Używane przez Game::checkCollisions po sprawdzeniu kolizji z zombi.
    bool hasReachedTarget() const;
};

#endif // PROJECTILE_H
