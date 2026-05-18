#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Gameobject.h"
#include "map.h"

class Game
{
private:
    sf::RenderWindow window;
    sf::Clock clock;
    std::vector<std::unique_ptr<GameObject>> objects;
    Map map;

public:
    Game();
    void run();
    void render();
    void update(float dt);
    void processEvents();
    void checkCollisions();

};

#endif // GAME_H
