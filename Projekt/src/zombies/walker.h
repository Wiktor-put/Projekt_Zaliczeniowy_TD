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
    // Mamy teraz 4 osobne paski animacji
    sf::Texture* texDown;
    sf::Texture* texUp;
    sf::Texture* texRight;
    sf::Texture* texLeft;

    sf::Sprite sprite; // Obiekt, który "założy" na siebie teksturę

    // --- ZMIENNE DO ANIMACJI ---
    int currentFrame = 0;       // Obecnie wyświetlana klatka w rzędzie (0-6)
    int totalFrames = 6;        // Całkowita liczba klatek w jednym rzędzie

    float animationTimer = 0.f; // Stoper odmierzający czas
    float frameDuration = 0.1f; // Czas wyświetlania jednej klatki w sekundach (10 klatek na sekundę)

public:
    Walker(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy funkcje z klasy bazowej, żeby narysować obrazek
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;
};


#endif // WALKER_H
