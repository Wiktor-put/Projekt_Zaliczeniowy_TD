#ifndef ROCKETTOWER_H
#define ROCKETTOWER_H

// rockettower.h — wyrzutnia rakiet zadająca obrażenia obszarowe (AoE).
#include "tower.h"

// Wieża wystrzeliwująca rakiety, które przy trafieniu ranią wszystkie zombie
// w promieniu aoeRadius. Skuteczna przeciwko grupom przeciwników.
class RocketTower : public Tower {
private:
    float aoeRadius;   // promień rażenia wybuchu rakiety (px)

public:
    // Inicjalizuje wieżę na podanej pozycji z wartościami z Config::RocketTower.
    RocketTower(sf::Vector2f pos);
    // Tworzy rakietę lecącą w aktualny cel (obrażenia obszarowe rozlicza Rocket::onHit).
    void shoot(std::vector<std::unique_ptr<GameObject>>& objects) override;
};

#endif // ROCKETTOWER_H