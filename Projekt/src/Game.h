#ifndef GAME_H
#define GAME_H

// Game.h — główna klasa gry, zarządza pętlą i wszystkimi obiektami sceny

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Gameobject.h"
#include "map.h"

// Odpowiada za okno, pętlę gry oraz integrację wszystkich podsystemów
// (mapa, obiekty, kolizje). Jeden egzemplarz na całą aplikację.
class Game
{
private:
    sf::RenderWindow window;                          // okno SFML
    sf::Clock clock;                                  // mierzy czas między klatkami
    std::vector<std::unique_ptr<GameObject>> objects; // wszystkie aktywne obiekty sceny
    Map map;                                          // wczytana mapa z waypointami i slotami

public:
    Game();

    // Uruchamia główną pętlę gry — blokuje do zamknięcia okna.
    void run();

    // Rysuje mapę i wszystkie obiekty sceny.
    void render();

    // Aktualizuje wszystkie obiekty i usuwa martwe; dt w sekundach.
    void update(float dt);

    // Obsługuje zdarzenia SFML (zamknięcie okna, klawiatura, mysz).
    void processEvents();

    // Wykrywa i reaguje na kolizje pocisków z zombie.
    void checkCollisions();

};

#endif // GAME_H
