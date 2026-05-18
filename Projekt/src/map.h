#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct TowerSlot {
    sf::Vector2f position;
    bool occupied;
};

class Map
{
private:
    int ID;
    std::vector<sf::Vector2f> waypoints;
    std::vector<TowerSlot> towerSlots;
public:
    Map() = default;
    void loadFromFile(const std::string& path);
    void draw(sf::RenderWindow& window) const;

    void occupiedSlot(TowerSlot& Slot);
    void freeSlot(TowerSlot& Slot);

    const std::vector<sf::Vector2f>& getWaypoints() const { return waypoints; }
    const std::vector<TowerSlot>& getSlots() const { return towerSlots; }
};

#endif // MAP_H
