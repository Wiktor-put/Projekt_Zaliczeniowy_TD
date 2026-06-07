#ifndef BUTTON_H
#define BUTTON_H

// button.h — pomocnicza klasa przycisku UI (prostokąt + wyśrodkowany napis).
// Używana w menu, ekranie wyboru mapy, pauzie, game over, ekranie wyników i HUD.

#include <SFML/Graphics.hpp>
#include <string>

// Lekki przycisk interfejsu. Świadomie NIE trzyma stanu "zaznaczenia" — o tym,
// czy jest podświetlony, decyduje wołający i przekazuje to do draw(). Dzięki temu
// klawiatura i mysz mogą współdzielić ten sam wybór bez dublowania logiki.
class Button {
private:
    sf::RectangleShape box;   // tło/ramka przycisku (origin ustawiony na środek)
    sf::Text label;           // napis wyśrodkowany na przycisku

    // Paleta kolorów — osobna dla stanu zwykłego i podświetlonego.
    sf::Color idleColor{40, 40, 40, 220};      // tło, gdy nieaktywny
    sf::Color hoverColor{70, 90, 50, 240};     // tło, gdy podświetlony
    sf::Color textColor{200, 200, 200};        // napis, gdy nieaktywny
    sf::Color textHoverColor{150, 255, 100};   // napis, gdy podświetlony

public:
    Button() = default;

    // Konfiguruje przycisk: czcionka, treść, środek, rozmiar i wielkość fontu.
    void setup(const sf::Font& font, const std::string& text,
               sf::Vector2f center, sf::Vector2f size, unsigned int charSize = 28);

    // Nadpisuje domyślną paletę kolorów (np. czerwony przycisk "Wyjdz").
    void setColors(sf::Color idle, sf::Color hover, sf::Color text, sf::Color textHover);

    // Zmienia napis (np. dynamiczny koszt wieży) z zachowaniem wyśrodkowania.
    void setText(const std::string& text);

    // Przesuwa przycisk (podajemy nowy środek).
    void setPosition(sf::Vector2f center);

    // True, gdy punkt (np. kursor myszy) leży wewnątrz przycisku.
    bool contains(sf::Vector2f point) const;

    // Rysuje przycisk; highlighted decyduje o palecie (hover vs idle).
    void draw(sf::RenderWindow& window, bool highlighted) const;
};

#endif // BUTTON_H
