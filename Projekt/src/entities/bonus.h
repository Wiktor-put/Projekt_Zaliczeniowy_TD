#ifndef BONUS_H
#define BONUS_H

#include "Gameobject.h"
#include <SFML/Graphics.hpp>

// 3 rodzaje bonusów
enum class BonusType { AMMO, MEDKIT, EMP };

class Bonus : public GameObject {
private:
    BonusType type;
    float lifetime;
    sf::RectangleShape shape;
    // Zmienne do spadania
    float targetY;
    bool isFalling;

public:
    // Zmieniony konstruktor (przyjmuje pozycję startową u góry ekranu i docelową na ziemi)
    Bonus(sf::Vector2f startPos, float targetY, BonusType type);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;

    BonusType getType() const { return type; }

    // Sprawdza, czy gracz kliknął dokładnie na to pudełko
    bool contains(sf::Vector2f point) const {
        return shape.getGlobalBounds().contains(point);
    }
};

#endif // BONUS_H