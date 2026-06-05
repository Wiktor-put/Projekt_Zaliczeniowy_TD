#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

class ResourceManager {
private:
    // Mapa przechowująca tekstury (kluczem jest ścieżka do pliku).
    // Używamy std::map, ponieważ gwarantuje ona, że referencje do tekstur nigdy nie wygasną w pamięci.
    static std::map<std::string, sf::Texture> textures;

public:
    // Zwraca referencję do tekstury. Jeśli jej nie ma, automatycznie ładuje ją z dysku.
    static sf::Texture& getTexture(const std::string& path);
};

#endif // RESOURCEMANAGER_H