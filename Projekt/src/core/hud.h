#ifndef HUD_H
#define HUD_H

// hud.h — interfejs gracza (górny pasek statystyk + panel budowy/ulepszeń wież).

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "button.h"

// Forward-deklaracje — pełne definicje potrzebne dopiero w hud.cpp.
class Player;
class WaveManager;
class Map;
class GameObject;

// Akcja zlecona przez kliknięcie w panel HUD (interpretowana przez Game).
enum class HudAction {
    NONE,
    BUY_TOWER,   // kup wieżę — patrz pole towerType (1..5)
    UPGRADE,     // ulepsz wieżę w zaznaczonym slocie
    SELL,         // sprzedaj wieżę w zaznaczonym slocie
    HELP         // akcja wywołania pomocy
};

// Wynik kliknięcia: rodzaj akcji + ewentualny typ wieży do kupienia.
struct HudClick {
    HudAction action = HudAction::NONE;
    int towerType = 0;  // istotne tylko dla BUY_TOWER
};

// Warstwa prezentacji i wejścia dla UI rozgrywki. Sama NIE przechowuje stanu gry —
// dane (gracz, fale, mapa, obiekty) dostaje przez parametry draw(). Posiada jedynie
// własne przyciski budowy/ulepszania/sprzedaży.
class HUD {
private:
    const sf::Font* font = nullptr;  // czcionka UI (właścicielem jest Game)

    Button buildButtons[5];  // wybór wieży (gdy slot wolny)
    Button upgradeButton;    // ulepszenie wieży (gdy slot zajęty)
    Button sellButton;       // sprzedaż wieży (gdy slot zajęty)
    Button helpButton;       // przycisk pomocy

public:
    HUD() = default;

    // Tworzy i pozycjonuje przyciski oraz zapamiętuje czcionkę. Wymaga wczytanego fontu.
    void init(const sf::Font& font);

    // Rysuje cały interfejs gracza: pasek statystyk, panel slotu i napis strefy zrzutu.
    void draw(sf::RenderWindow& window,
              const Player& player, const WaveManager& wave, const Map& map,
              const std::vector<std::unique_ptr<GameObject>>& objects,
              int selectedSlotIndex) const;

    // Obsługuje kliknięcie w panel wież (gdy slot jest zaznaczony). Parametr occupied
    // decyduje, które przyciski są aktywne. Zwraca akcję do wykonania przez Game.
    HudClick handleClick(sf::Vector2f mouse, bool occupied) const;
};

#endif // HUD_H
