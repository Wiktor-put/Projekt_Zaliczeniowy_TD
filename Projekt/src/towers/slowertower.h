#ifndef SLOWERTOWER_H
#define SLOWERTOWER_H
// slowertower.h — wieża spowalniająca zombie kulami śnieżnymi.
#include "tower.h"

// Wieża wsparcia: nie zabija szybko, ale jej pociski (Snowball) spowalniają
// trafione zombie, dając pozostałym wieżom więcej czasu na ostrzał.
class SlowerTower : public Tower {
private:
    float slowDuration;  // czas trwania spowolnienia nałożonego na cel (s)
public:
    // Inicjalizuje wieżę na podanej pozycji z wartościami z Config::SlowerTower.
    SlowerTower(sf::Vector2f pos);
    // Tworzy kulę śnieżną (Snowball), która spowalnia trafione zombie.
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif