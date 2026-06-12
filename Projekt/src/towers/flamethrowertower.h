#ifndef FLAMETHROWERTOWER_H
#define FLAMETHROWERTOWER_H
// flamethrowertower.h — miotacz ognia podpalający zombie (obrażenia w czasie).
#include "tower.h"

// Wieża na bliski dystans: jej pociski podpalają zombie, zadając obrażenia
// przez kilka sekund (DOT) zamiast jednego mocnego trafienia.
class FlamethrowerTower : public Tower {
private:
    int dps;             // obrażenia podpalenia na sekundę
    float burnDuration;  // jak długo cel się pali (s)
public:
    // Inicjalizuje wieżę na podanej pozycji z wartościami z Config::FlamethrowerTower.
    FlamethrowerTower(sf::Vector2f pos);
    // Tworzy pocisk ognia (Flame), który podpala trafione zombie.
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif