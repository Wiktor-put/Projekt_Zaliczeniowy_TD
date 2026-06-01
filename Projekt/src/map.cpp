#include "map.h"

void Map::loadFromFile(const std::string& path) {
    waypoints.clear();
    towerSlots.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku: " << path << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        char type;
        float x, y;

        if (iss >> type >> x >> y) {
            if (type == 'W') {
                waypoints.push_back(sf::Vector2f(x, y));
            } else if (type == 'S') {
                towerSlots.push_back({sf::Vector2f(x, y), false});
            } else {
                std::cerr << "Nieznany typ '" << type << "' w linii " << lineNumber << std::endl;
            }
        } else {
            std::cerr << "Bledny format w linii " << lineNumber << ": " << line << std::endl;
        }
    }

    std::cout << "Wczytano " << waypoints.size() << " waypointow i "
              << towerSlots.size() << " slotow z " << path << std::endl;
}

void Map::draw(sf::RenderWindow& window) const {
    if (waypoints.size() < 2) return;

    // rysowanie ścieżki jako linii łączącej waypointy
    sf::VertexArray path(sf::LineStrip, waypoints.size());
    for (size_t i = 0; i < waypoints.size(); ++i) {
        path[i].position = waypoints[i];
        path[i].color = sf::Color(100, 60, 40);  // brązowa droga
    }
    window.draw(path);

    // rysowanie slotów jako szare kwadraty
    for (const auto& slot : towerSlots) {
        sf::RectangleShape shape(sf::Vector2f(30.f, 30.f));
        shape.setPosition(slot.position - sf::Vector2f(15.f, 15.f));
        shape.setFillColor(sf::Color(60, 60, 60, 100));
        shape.setOutlineColor(sf::Color(120, 120, 120));
        shape.setOutlineThickness(1.f);
        window.draw(shape);
    }
}

int Map::getSlotAt(sf::Vector2f pos) const {
    constexpr float HALF_SIZE = 20.f;  // połowa rozmiaru slotu (40x40)

    for (size_t i = 0; i < towerSlots.size(); ++i) {
        sf::Vector2f slotPos = towerSlots[i].position;

        // Sprawdź czy pos jest w prostokącie wokół środka slotu
        if (pos.x >= slotPos.x - HALF_SIZE && pos.x <= slotPos.x + HALF_SIZE &&
            pos.y >= slotPos.y - HALF_SIZE && pos.y <= slotPos.y + HALF_SIZE) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Map::occupiedSlot(TowerSlot& Slot){
    Slot.occupied = true;
}
