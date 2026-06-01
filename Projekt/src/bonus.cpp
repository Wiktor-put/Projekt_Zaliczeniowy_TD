#include "bonus.h"
#include "Config.h"

Bonus::Bonus(sf::Vector2f pos, BonusType type) : type(type) {
    position = pos;
    alive = true;
    lifetime = Config::BONUS_LIFETIME; // Żyje 6 sekund

    // Rysujemy pudełko z bonusem 20x20 pikseli
    shape.setSize(sf::Vector2f(20.f, 20.f));
    shape.setOrigin(10.f, 10.f);
    shape.setPosition(position);

    // Kolor zależy od typu (Amunicja=Złota, Apteczka=Zielona, EMP=Błękitne)
    if (type == BonusType::AMMO) shape.setFillColor(sf::Color::Yellow);
    else if (type == BonusType::MEDKIT) shape.setFillColor(sf::Color::Green);
    else if (type == BonusType::EMP) shape.setFillColor(sf::Color::Cyan);
}

void Bonus::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    (void)objects;
    lifetime -= dt;
    // Jeżeli czas minął, a gracz nie kliknął pudełka - bonus przepada
    if (lifetime <= 0.f) {
        destroy();
    }
}

void Bonus::render(sf::RenderWindow& window) {
    window.draw(shape);
}