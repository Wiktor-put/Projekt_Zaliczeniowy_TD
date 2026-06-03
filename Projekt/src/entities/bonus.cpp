#include "bonus.h"
#include "Config.h"

Bonus::Bonus(sf::Vector2f startPos, float targetY, BonusType type)
    : type(type), targetY(targetY), isFalling(true)
{
    position = startPos;
    alive = true;
    lifetime = Config::BONUS_LIFETIME;

    shape.setSize(sf::Vector2f(20.f, 20.f));
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(position);

    if (type == BonusType::AMMO) shape.setFillColor(sf::Color::Yellow);
    else if (type == BonusType::MEDKIT) shape.setFillColor(sf::Color::Green);
    else if (type == BonusType::EMP) shape.setFillColor(sf::Color::Cyan);
}

void Bonus::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;

    if (isFalling) {
        // Pudełko leci w dół
        position.y += Config::BONUS_FALL_SPEED * dt;

        // Jeśli dotarło do ziemi (targetY)
        if (position.y >= targetY) {
            position.y = targetY;
            isFalling = false; // Zatrzymuje się
        }
        shape.setPosition(position);
    } else {
        // Licznik czasu odpalany DOPIERO gdy paczka wyląduje!
        lifetime -= dt;
        if (lifetime <= 0.f) {
            destroy();
        }
    }
}

void Bonus::render(sf::RenderWindow& window) {
    window.draw(shape);
}