#ifndef BONUS_H
#define BONUS_H

#include "Gameobject.h"
#include <SFML/Graphics.hpp>

// bonus.h — klikalny bonus spadający nad strefą zrzutu (drop zone).
// 3 rodzaje: AMMO (kasa), MEDKIT (życie), EMP (ogłuszenie wszystkich zombie).
enum class BonusType { AMMO, MEDKIT, EMP };

// Obiekt bonusu: spada z góry ekranu do wyznaczonej wysokości, po czym czeka na
// kliknięcie gracza. Efekt po zebraniu rozstrzyga Game::applyBonus.
class Bonus : public GameObject {
private:
    BonusType type;      // rodzaj bonusu (AMMO/MEDKIT/EMP)
    float lifetime;      // pozostały czas, zanim bonus zniknie sam
    sf::Sprite sprite;   // grafika bonusu (ikona zależna od typu)
    // Zmienne do spadania
    float targetY;       // docelowa wysokość, na której bonus się zatrzymuje
    bool isFalling;      // true gdy bonus jeszcze spada w dół

public:
    // Konstruktor przyjmuje pozycję startową u góry ekranu i docelową wysokość lądowania.
    Bonus(sf::Vector2f startPos, float targetY, BonusType type);

    // Animuje spadanie, odlicza lifetime i niszczy bonus po jego upływie.
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    // Rysuje ikonę bonusu.
    void render(sf::RenderWindow& window) override;

    BonusType getType() const { return type; }

    // Sprawdza, czy gracz kliknął dokładnie na to pudełko
    bool contains(sf::Vector2f point) const {
        return sprite.getGlobalBounds().contains(point);
    }
};

#endif // BONUS_H