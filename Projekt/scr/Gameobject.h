#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class GameObject
{
protected:
    sf::Vector2f position;      // pozycja w pikselach
    sf::Vector2f velocity;      // prędkość w px/s
    float rotation;             // aktualny obrót w stopniach
    float rotationSpeed;        // prędkość obrotu w deg/s
    bool alive;                 // czy obiekt jeszcze istnieje
public:
    GameObject();
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }
    sf::Vector2f getPosition() const { return position; }

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};

#endif // GAMEOBJECT_H
