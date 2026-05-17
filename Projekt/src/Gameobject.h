#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
protected:
    sf::Vector2f position{0.f, 0.f};
    sf::Vector2f velocity{0.f, 0.f};
    float rotation = 0.f;
    float rotationSpeed = 0.f;
    bool alive = true;
public:
    GameObject() = default;
    ~GameObject() = default;
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }
    sf::Vector2f getPosition() const { return position; }

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};

#endif // GAMEOBJECT_H
