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
#include "bonus.h"
#include "wavemanager.h"
#include "button.h"
#include "hud.h"

enum class GameState {
    MENU,          // ekran startowy z opcjami
    MAP_SELECT,    // ekran wyboru mapy przed rozpoczeciem gry
    PLAYING,       // wlasciwa rozgrywka
    PAUSED,        // pauza w trakcie gry
    GAME_OVER,     // ekran konca gry z wynikiem
    HIGHSCORES,     // tablica top 10 wynikow
    HELP            // ekran instrukcji
};

struct FlyingIcon {
    sf::Sprite sprite;
    sf::Vector2f startPos;
    sf::Vector2f targetPos;
    float progress = 0.f;  // od 0.0 (start) do 1.0 (koniec lotu)
};

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
    GameState state;                                  // stan gry
    GameState previousState;                          // Zapamiętuje, skąd weszliśmy w Pomoc
    std::string currentMapPath;                       // ścieżka aktualnie wybranej mapy (względna)
    int mapSelectedOption = 0;                        // podświetlona mapa na ekranie wyboru
    sf::Font font;
    WaveManager waveManager;    // potrzebny do napisu GAME OVER

    // --- PRZYCISKI UI (klasa Button) ---
    Button menuButtons[5];            // Nowa gra / Wczytaj / Wyniki / Wyjdz / Zasady gry
    std::vector<Button> mapButtons;   // jeden przycisk na mapę z Config::Maps
    Button pauseButtons[2];           // 0 = wznow, 1 = menu
    Button gameOverButtons[2];        // 0 = zagraj ponownie, 1 = menu
    Button hsBackButton;              // powrot z ekranu wynikow
    HUD hud;                          // interfejs gracza (pasek statystyk + panel wież)

    std::vector<FlyingIcon> flyingIcons; // Lista animowanych ikon lecących do HUD
    float freezeTintTimer = 0.f;         // Czas trwania błękitnego błysku

    // Tworzy i pozycjonuje przyciski ekranów (menu/pauza/itp.). Wołane w konstruktorze.
    void initButtons();

public:
    Game();
    // Tworzenie nowej rozgrywki
    void startNewGame();

    // Uruchamia główną pętlę gry — blokuje do zamknięcia okna.
    void run();

    // Rysuje mapę i wszystkie obiekty sceny.
    void render();

    void renderMenu();
    // Ekran wyboru mapy (lista plansz z Config::Maps).
    void renderMapSelect();
    void renderPlaying();
    void renderPaused();
    void renderGameOver();
    void renderHighscores();
    void renderHelp();

    // Ogolna aktualizacja stanu dla wszystkich stanow gry
    void update(float dt);
    // Aktualizuje wszystkie obiekty i usuwa martwe; dt w sekundach. Podczas trawania gry
    void updatePlaying(float dt);

    // Obsługuje zdarzenia SFML (zamknięcie okna, klawiatura, mysz).
    void processEvents();

    // Wykrywa i reaguje na kolizje pocisków z zombie.
    void checkCollisions();

    // Próbuje kupić wieżę danego typu w zaznaczonym slocie.
    // type: 1=MachineGun (kolejne typy dojdą w MS3).
    // Odejmuje koszt od gracza i zajmuje slot. Czyści selectedSlotIndex po zakupie.
    void tryBuyTower(int type);

    void tryUpgradeTower();
    void trySellTower();

    //zapisywanie gry
    void saveGame();
    void loadGame();

    // Zeruje martwe targety wież i usuwa martwe obiekty z kontenera.
    // Wywoływane na końcu update() — po update i kolizjach.
    void removeDeadObjects();

    void applyBonus(BonusType type, sf::Vector2f startPos);

    int menuSelectedOption;  // ktora opcja menu jest podswietlona (0-3)
    void handleMenuChoice(int option);
};

#endif // GAME_H


