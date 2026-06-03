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

    // --- ZMIENNE DO ANIMACJI ---
    int currentFrame = 0;       // Obecnie wyświetlana klatka w rzędzie (0-7)
    int currentRow = 0;         // Obecny rząd (kierunek: 0=dół, 1=góra, 2=prawo, 3=lewo)
    int totalFrames = 8;        // Całkowita liczba klatek w jednym rzędzie

    float animationTimer = 0.f; // Stoper odmierzający czas
    float frameDuration = 0.1f; // Czas wyświetlania jednej klatki w sekundach (10 klatek na sekundę)

    int frameWidth;             // Szerokość pojedynczej klatki
    int frameHeight;            // Wysokość pojedynczej klatki
public:
    Walker(const std::vector<sf::Vector2f>& waypoints);

    // Nadpisujemy funkcje z klasy bazowej, żeby narysować obrazek
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) override;
    void render(sf::RenderWindow& window) override;
};


#endif // WALKER_H
