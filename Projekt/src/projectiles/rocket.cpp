#include "rocket.h"
#include "Config.h"
#include "resourcemanager.h"
#include <cmath>

Rocket::Rocket(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float aoeRadius)
    : Projectile(startPos, targetPos, damage), aoeRadius(aoeRadius)
{
    // Rakiety lecą wolniej niż kule snajpera
    speed = Config::ROCKET_SPEED;
    velocity *= speed;

    // Tekstura rakiety. rotation jest wyliczone w Projectile (kierunek lotu),
    // a tekstura narysowana jest "dziobem do góry", więc korygujemy offsetem.
    const sf::Texture& tex = ResourceManager::getTexture(Config::Assets::ROCKET_PROJECTILE);
    sprite.setTexture(tex, true);
    sprite.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
    sprite.setScale(0.6f, 0.6f);
    sprite.setPosition(position);
    sprite.setRotation(rotation + Config::TOWER_TEXTURE_FORWARD);
}

void Rocket::render(sf::RenderWindow& window) {
    sprite.setPosition(position);  // pozycja zmienia się co klatkę; obrót jest stały
    window.draw(sprite);
}

void Rocket::onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) {
    explosion(objects);
    // 3. Po wybuchu rakieta ulega zniszczeniu
    destroy();
}

void Rocket::explosion(std::vector<std::unique_ptr<GameObject>>& objects){
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
}
