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
        // Jeśli nie istnieje, musimy ją załadować z dysku!
        // Ładujemy obrazek BEZPOŚREDNIO do słownika, automatycznie doklejając ASSETS_DIR
        if (!textures[path].loadFromFile(std::string(ASSETS_DIR) + path)) {
            std::cerr << "Nie udalo sie zaladowac tekstury: " << path << std::endl;
        }
    }

    // Zwracamy teksturę
    return textures[path];
}