#ifndef MAP_H
#define MAP_H

// map.h — wczytywanie mapy z pliku, rysowanie ścieżki i zarządzanie slotami wież

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Config.h"

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
    sf::Vector2f dropZonePos{1150.f, 600.f}; // Domyślna pozycja drop zone

    // Ścieżka do tła TEJ mapy — pozwala każdej planszy mieć własną grafikę
    // (ustawiana w pliku mapy linią "B <ścieżka>"; domyślnie wartość z Config).
    std::string backgroundPath = Config::Assets::BACKGROUND;

    // --- WIZUALIA ZBUDOWANE RAZ PRZY WCZYTANIU (nie co klatkę) ---
    // Trzymanie gotowych sprite'ów eliminuje per-klatkowe lookupy w ResourceManager
    // (std::map po stringu) oraz alokacje kształtów w każdej klatce draw().
    sf::Sprite bgSprite;                     // tło planszy
    sf::Sprite dzSprite;                     // strefa zrzutu (drop zone)
    std::vector<sf::CircleShape> pathJoints; // okrągłe "przeguby" na zakrętach
    std::vector<sf::RectangleShape> pathSegments; // proste odcinki drogi
    std::vector<sf::Sprite> gateSprites;     // ściany/brama bazy

    // Buduje sprite'y i geometrię ścieżki na podstawie wczytanych danych.
    // Wołane na końcu loadFromFile — tu dzieją się wszystkie kosztowne operacje.
    void buildVisuals();

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
    std::vector<TowerSlot>& getSlots() { return towerSlots; }
    int getSlotAt(sf::Vector2f pos) const;

    sf::Vector2f getDropZonePos() const { return dropZonePos; }
};

#endif // MAP_H
