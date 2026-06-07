#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Gameobject.h — abstrakcyjna klasa bazowa dla wszystkich obiektów gry

#include <iostream>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// Wspólny interfejs dla wszystkich bytów na scenie (zombie, wieże, pociski).
// Każdy obiekt ma pozycję, prędkość i flagę życia; logika i rendering są wirtualne.
class GameObject
{
protected:
    sf::Vector2f position{0.f, 0.f};   // pozycja na ekranie w pikselach
    sf::Vector2f velocity{0.f, 0.f};   // wektor prędkości (px/s)
    float rotation = 0.f;              // kąt obrotu w stopniach
    float rotationSpeed = 0.f;         // prędkość obrotu (stopnie/s)
    bool alive = true;                 // false = obiekt do usunięcia z listy
public:
    GameObject() = default;
    // Wirtualny destruktor — obiekty są usuwane polimorficznie przez
    // unique_ptr<GameObject>, więc bez 'virtual' destruktory klas pochodnych
    // nie zostałyby wywołane (niezdefiniowane zachowanie).
    virtual ~GameObject() = default;

    // Zwraca false gdy obiekt należy usunąć z listy sceny.
    bool isAlive() const { return alive; }

    // Oznacza obiekt do usunięcia na końcu bieżącej klatki.
    void destroy() { alive = false; }

    sf::Vector2f getPosition() const { return position; }

    // Aktualizuje stan obiektu — dt w sekundach; obiekty mogą dodawać nowe do listy.
    virtual void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects) = 0;

    // Rysuje obiekt na podanym oknie.
    virtual void render(sf::RenderWindow& window) = 0;
};

#endif // GAMEOBJECT_H
