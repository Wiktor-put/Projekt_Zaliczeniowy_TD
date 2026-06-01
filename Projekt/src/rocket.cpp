#include "rocket.h"
#include "Config.h"
#include <cmath>

Rocket::Rocket(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float aoeRadius)
    : Projectile(startPos, targetPos, damage), aoeRadius(aoeRadius)
{
    // Rakiety lecą wolniej niż kule snajpera
    speed = Config::ROCKET_SPEED;
    velocity *= speed;

    // Rakieta będzie odrobinę większa i czerwona
    shape.setRadius(6.f);
    shape.setOrigin(6.f, 6.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(position);
}

void Rocket::onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) {
    // 1. Ustalenie środka wybuchu (tam gdzie znajduje się rakieta w momencie trafienia)
    sf::Vector2f explosionCenter = position;

    // 2. Przeszukujemy wszystkie obiekty w grze
    for (auto& obj : objects) {
        Zombie* z = dynamic_cast<Zombie*>(obj.get());
        if (z && z->isAlive()) {

            // Obliczamy odległość zombiaka od środka wybuchu (korzystamy z kwadratu odległości dla optymalizacji)
            sf::Vector2f diff = z->getPosition() - explosionCenter;
            float distSq = diff.x * diff.x + diff.y * diff.y;

            // Jeśli zombiak jest w promieniu rażenia, obrywa wybuchem!
            if (distSq <= aoeRadius * aoeRadius) {
                z->takeDamage(damage, DamageType::EXPLOSION);
            }
        }
    }

    // 3. Po wybuchu rakieta ulega zniszczeniu
    destroy();
}