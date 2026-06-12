#ifndef SNIPERTOWER_H
#define SNIPERTOWER_H

#include "tower.h"

// snipertower.h — wieża snajperska: ogromny zasięg i obrażenia, bardzo wolny ostrzał.
class SniperTower : public Tower
{
public:
    // Inicjalizuje wieżę na podanej pozycji z wartościami z Config::SniperTower.
    SniperTower(sf::Vector2f pos);
    // Tworzy pojedynczy, mocny pocisk skierowany w aktualny cel.
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
    void upgrade(); // Nadpisana, by skalować grafikę
};

#endif // SNIPERTOWER_H