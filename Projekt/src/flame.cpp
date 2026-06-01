#include "flame.h"

Flame::Flame(sf::Vector2f startPos, sf::Vector2f targetPos, int dps, float duration)
    : Projectile(startPos, targetPos, 0), dps(dps), duration(duration) { // bazowe damage=0, bo bije w czasie
    speed = 400.f;
    velocity *= speed;
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color(255, 100, 0)); // Pomarańczowy
    shape.setPosition(position);
}

void Flame::onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) {
    hitZombie->applyBurn(dps, duration); // Nakłada efekt podpalenia
    destroy();
}