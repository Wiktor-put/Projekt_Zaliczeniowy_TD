#ifndef AMORED_H
#define AMORED_H


#include "zombie.h"

// --- Klasa Armored (Wolny, odporny na kule) ---
class Armored : public Zombie
{
private:
    // tekstury
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
    float frameDuration = 0.1f;
public:
    Armored(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy funkcję otrzymywania obrażeń, aby uwzględnić pancerz
    void takeDamage(int dmg, DamageType type) override;
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;
};

#endif //