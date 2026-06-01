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
#include "player.h"

// Odpowiada za okno, pętlę gry oraz integrację wszystkich podsystemów
// (mapa, obiekty, kolizje). Jeden egzemplarz na całą aplikację.
class Game
{
private:
    sf::RenderWindow window;                          // okno SFML
    sf::Clock clock;                                  // mierzy czas między klatkami
    std::vector<std::unique_ptr<GameObject>> objects; // wszystkie aktywne obiekty sceny
    Map map;                                          // wczytana mapa z waypointami i slotami
    Player player;                                    // stan gracza (życia, waluta, wynik)
    float spawnTimer = 0.f;
    int selectedSlotIndex = -1;                       // indeks klikniętego slotu, -1 = brak wyboru

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

    // Próbuje kupić wieżę danego typu w zaznaczonym slocie.
    // type: 1=MachineGun (kolejne typy dojdą w MS3).
    // Odejmuje koszt od gracza i zajmuje slot. Czyści selectedSlotIndex po zakupie.
    void tryBuyTower(int type);

    // Zeruje martwe targety wież i usuwa martwe obiekty z kontenera.
    // Wywoływane na końcu update() — po update i kolizjach.
    void removeDeadObjects();

};

#endif // GAME_H


