#include "resourcemanager.h"

// Definicja makra (zabezpieczenie, gdyby qmake go nie przekazał)
#ifndef ASSETS_DIR
#define ASSETS_DIR "."
#endif

// Inicjalizacja statycznej mapy
std::map<std::string, sf::Texture> ResourceManager::textures;

sf::Texture& ResourceManager::getTexture(const std::string& path) {
    // Sprawdzamy, czy tekstura pod podaną ścieżką JUŻ ISTNIEJE w naszej mapie
    if (textures.find(path) == textures.end()) {
        // Jeśli nie istnieje, ładujemy obrazek z dysku prosto do słownika,
        // automatycznie doklejając ASSETS_DIR. Ewentualny błąd ładowania
        // skutkuje pustą teksturą (SFML rysuje wtedy biały prostokąt).
        textures[path].loadFromFile(std::string(ASSETS_DIR) + path);
    }

    // Zwracamy teksturę
    return textures[path];
}