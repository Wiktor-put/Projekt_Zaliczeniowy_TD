#include "zombie.h"
#include "Config.h"

Zombie::Zombie() {}

void Zombie::update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) {
    // Tutaj w przyszłości będzie logika poruszania się po Waypointach.
    // Na razie zombie może po prostu przesuwać się w prawo.
    position.x += speed * dt;
    shape.setPosition(position);
}

void Zombie::render(sf::RenderWindow& window) {
    window.draw(shape);
}


Walker::Walker(sf::Vector2f startPosition) {
    // 1. Ustawienie pozycji początkowej (zmienna z GameObject)
    position = startPosition;
    alive = true;

    // 2. Pobranie statystyk z przestrzeni Config::Walker
    hp = Config::Walker::HP;
    speed = Config::Walker::SPEED;
    value = Config::Walker::REWARD;

    // 3. Ustawienie testowego wyglądu dla Walkera (np. zielony kwadrat)
    shape.setSize(sf::Vector2f(30.f, 30.f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(15.f, 15.f); // Ustawiamy środek kwadratu
    shape.setPosition(position);
}