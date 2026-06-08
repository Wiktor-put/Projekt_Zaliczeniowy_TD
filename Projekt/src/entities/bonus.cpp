#include "bonus.h"
#include "Config.h"
#include "resourcemanager.h"

Bonus::Bonus(sf::Vector2f startPos, float targetY, BonusType type)
    : type(type), targetY(targetY), isFalling(true)
{
    position = startPos;
    alive = true;
    lifetime = Config::BONUS_LIFETIME;

    // Przypisanie odpowiedniej grafiki w zależności od rodzaju bonusu
    if (type == BonusType::AMMO) {
        sprite.setTexture(ResourceManager::getTexture(Config::Assets::ICON_COIN), true);
    } else if (type == BonusType::MEDKIT) {
        sprite.setTexture(ResourceManager::getTexture(Config::Assets::ICON_HEART), true);
    } else if (type == BonusType::EMP) {
        sprite.setTexture(ResourceManager::getTexture(Config::Assets::ICON_ICE), true);
    }

    // Skalujemy ikonkę do sensownego rozmiaru (np. szerokość 30 pikseli)
    float scale = 28.f / sprite.getTexture()->getSize().x;
    sprite.setScale(scale, scale);

    // Ustawiamy środek obrotu/rysowania na sam środek obrazka
    sprite.setOrigin(sprite.getTexture()->getSize().x / 2.f, sprite.getTexture()->getSize().y / 2.f);
    sprite.setPosition(position);
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
        sprite.setPosition(position);
    } else {
        // Licznik czasu odpalany DOPIERO gdy paczka wyląduje!
        lifetime -= dt;
        if (lifetime <= 0.f) {
            destroy();
        }
    }
}

void Bonus::render(sf::RenderWindow& window) {
    window.draw(sprite);
}