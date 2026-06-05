#ifndef TANK_H
#define TANK_H

#include "zombie.h"

// --- Klasa Tank (Bardzo wolny, ogromna ilość HP) ---
class Tank : public Zombie
{
private:
    static sf::Texture texDown;
    static sf::Texture texUp;
    static sf::Texture texRight;
    static sf::Texture texLeft;
    static bool areTexturesLoaded;

    sf::Sprite sprite;

    // --- ZMIENNE DO ANIMACJI ---
    int currentFrame = 0;
    int totalFrames = 8;
    float animationTimer = 0.f;
    float frameDuration = 0.15f; // Wydłużony czas = ociężały, ciężki chód
public:
    Tank(const std::vector<sf::Vector2f>& waypoints);

    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;
};


#endif //