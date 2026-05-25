#ifndef MAP_H
#define MAP_H

// map.h — wczytywanie mapy z pliku, rysowanie ścieżki i zarządzanie slotami wież

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Pojedynczy slot, w którym gracz może postawić wieżę.
struct TowerSlot {
    sf::Vector2f position; // środek slotu na ekranie
    bool occupied;         // true gdy stoi na nim wieża
};

// Przechowuje dane mapy: ścieżkę zombie (waypoints) i miejsca na wieże (slots).
// Wczytywana z pliku tekstowego; rysuje ścieżkę i sloty bezpośrednio na oknie.
class Map
{
private:
    int ID;                              // identyfikator mapy (do rozróżniania plansz)
    std::vector<sf::Vector2f> waypoints; // kolejne punkty trasy zombie
    std::vector<TowerSlot> towerSlots;   // dostępne miejsca na wieże
public:
    Map() = default;

    // Wczytuje waypointy i sloty z pliku tekstowego o podanej ścieżce.
    void loadFromFile(const std::string& path);

    // Rysuje ścieżkę zombie i sloty na wieże na podanym oknie.
    void draw(sf::RenderWindow& window) const;

    // Oznacza slot jako zajęty po postawieniu wieży.
    void occupiedSlot(TowerSlot& Slot);

    // Zwalnia slot po sprzedaniu lub zniszczeniu wieży.
    void freeSlot(TowerSlot& Slot);

    const std::vector<sf::Vector2f>& getWaypoints() const { return waypoints; }
    const std::vector<TowerSlot>& getSlots() const { return towerSlots; }
};

#endif // MAP_H
