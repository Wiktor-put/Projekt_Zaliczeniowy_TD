#ifndef WALKER_H
#define WALKER_H

// walker.h — podstawowy typ zombie poruszający się po ścieżce

#include "zombie.h"
#include <SFML/Graphics.hpp>

// Najsłabszy wariant zombie: niskie HP, niska prędkość, mała nagroda.
// Statystyki wczytywane z Config::Walker.
class Walker : public Zombie
{
private:
    // Statyczne zmienne - jeden obrazek w pamięci współdzielony przez wszystkie Walkery
    static sf::Texture texture;
    static bool isTextureLoaded;

    sf::Sprite sprite; // Obiekt, który "założy" na siebie teksturę
public:
    Walker(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy funkcje z klasy bazowej, żeby narysować obrazek
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;
};


#endif // WALKER_H
