#ifndef MACHINEGUNTOWER_H
#define MACHINEGUNTOWER_H

// machineguntower.h — szybkostrzelna wieża z karabinem maszynowym

#include "tower.h"

// Najtańsza wieża ofensywna: wysoki fire rate, niskie obrażenia jednostkowe.
// Nadpisuje shoot() by tworzyć pociski wg parametrów z Config::MachineGunTower.
class MachineGunTower: public Tower
{
public:
    // Inicjalizuje wieżę na podanej pozycji z wartościami z Config::MachineGunTower.
    MachineGunTower(sf::Vector2f pos);

    // Tworzy pojedynczy pocisk skierowany w aktualny cel i dodaje go do sceny.
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};

#endif // MACHINEGUNTOWER_H
