#include "snowball.h"
#include "Config.h"

Snowball::Snowball(sf::Vector2f startPos, sf::Vector2f targetPos, float duration)
    : Projectile(startPos, targetPos, 0), duration(duration) {
    speed = Config::SNOWBALL_SPEED;
    velocity *= speed;
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color(100, 200, 255)); // Lodowy, błękitny
    shape.setPosition(position);
}

void Snowball::onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) {
    hitZombie->applySlow(duration); // Nakłada efekt spowolnienia
    destroy();
}