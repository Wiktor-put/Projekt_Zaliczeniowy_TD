#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
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
    std::vector<sf::Vector2f> waypoint;
    std::vector<TowerSlot> towerSlots;
public:
    Map();
    void loadFromFile(const std::string& path);
    void draw(sf::RenderWindow& window);

    void occupiedSlot(TowerSlot& Slot);
    void freeSlot(TowerSlot& Slot);
};

#endif // MAP_H
