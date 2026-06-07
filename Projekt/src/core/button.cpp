#include "button.h"

void Button::setup(const sf::Font& font, const std::string& text,
                   sf::Vector2f center, sf::Vector2f size, unsigned int charSize) {
    box.setSize(size);
    box.setOrigin(size.x / 2.f, size.y / 2.f);   // origin = środek, łatwe centrowanie
    box.setOutlineThickness(2.f);

    label.setFont(font);
    label.setCharacterSize(charSize);
    label.setString(text);

    setPosition(center);  // ustawia pozycję pudełka i wyśrodkowuje napis
}

void Button::setColors(sf::Color idle, sf::Color hover, sf::Color text, sf::Color textHover) {
    idleColor = idle;
    hoverColor = hover;
    textColor = text;
    textHoverColor = textHover;
}

void Button::setText(const std::string& text) {
    label.setString(text);
    setPosition(box.getPosition());  // ponów wyśrodkowanie po zmianie treści
}

void Button::setPosition(sf::Vector2f center) {
    box.setPosition(center);
    // Uwzględniamy offset bounds (left/top) — font dokłada własny margines,
    // więc samo width/2 nie wystarcza do idealnego wyśrodkowania.
    sf::FloatRect b = label.getLocalBounds();
    label.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    label.setPosition(center);
}

bool Button::contains(sf::Vector2f point) const {
    return box.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderWindow& window, bool highlighted) const {
    // Rysujemy na kopiach, by metoda mogła pozostać const, a wygląd zależeć od stanu.
    sf::RectangleShape b = box;
    sf::Text t = label;

    b.setFillColor(highlighted ? hoverColor : idleColor);
    b.setOutlineColor(highlighted ? sf::Color(150, 255, 100) : sf::Color(100, 100, 100));
    t.setFillColor(highlighted ? textHoverColor : textColor);
    if (highlighted) t.setStyle(sf::Text::Bold);

    window.draw(b);
    window.draw(t);
}
