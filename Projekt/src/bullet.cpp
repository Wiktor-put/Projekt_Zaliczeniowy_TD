#include "Bullet.h"
#include "Config.h"

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos, int damage)
    : Projectile(startPos, targetPos, damage)
{
    speed = Config::BULLET_SPEED;
    velocity *= speed;  // jednostkowy z bazy * prędkość = velocity w px/s

    shape.setRadius(4.f);
    shape.setOrigin(4.f, 4.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
}

void Bullet::onHit(Zombie* hitZombie,
                   std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;  // BULLET nie potrzebuje kontenera - trafia tylko jednego

    hitZombie->takeDamage(damage, DamageType::BULLET);
    destroy();
}
