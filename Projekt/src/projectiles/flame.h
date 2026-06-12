#ifndef FLAME_H
#define FLAME_H
#include "projectile.h"

// Flame — pocisk miotacza ognia. Po trafieniu podpala zombie (obrażenia w czasie),
// zamiast zadawać jednorazowe obrażenia jak Bullet.
class Flame : public Projectile {
private:
    int dps;         // obrażenia podpalenia na sekundę
    float duration;  // czas trwania podpalenia (s)
public:
    Flame(sf::Vector2f startPos, sf::Vector2f targetPos, int dps, float duration);
    // Nakłada na trafione zombie efekt podpalenia (applyBurn) i niszczy pocisk.
    void onHit(Zombie* hitZombie, std::vector<std::unique_ptr<GameObject>>& objects) override;
};
#endif