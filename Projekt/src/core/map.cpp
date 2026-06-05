#include "map.h"
#include "Config.h"

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
            }
        } else if (type == 'D') {
            // zapisujemy pozycję strefy zrzutu
            dropZonePos = sf::Vector2f(x, y);

        } else {
            std::cerr << "Nieznany typ '" << type << "' w linii " << lineNumber << std::endl;
        }
    }

    std::cout << "Wczytano " << waypoints.size() << " waypointow i "
              << towerSlots.size() << " slotow z " << path << std::endl;

    // --- ŁADOWANIE TŁA MAPY ---
    if (!isBgLoaded) {
        if (bgTexture.loadFromFile(std::string(ASSETS_DIR) + Config::Assets::BACKGROUND)) {
            bgSprite.setTexture(bgTexture);

            // Kod automatycznie skaluje obrazek, żeby idealnie pasował do okna 1280x720
            float scaleX = static_cast<float>(Config::WINDOW_WIDTH) / bgTexture.getSize().x;
            float scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / bgTexture.getSize().y;
            bgSprite.setScale(scaleX, scaleY);

            isBgLoaded = true;
        } else {
            std::cerr << "Nie udalo sie zaladowac tla" << std::endl;
        }
    }
}

void Map::draw(sf::RenderWindow& window) const {
    if (waypoints.size() < 2) return;

    // --- RYSOWANIE TŁA MAPY (Zawsze na samym spodzie!) ---
    if (isBgLoaded) {
        window.draw(bgSprite);
    }

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

    // --- RYSOWANIE DROP ZONE (Tylko grafika terenu) ---
    sf::RectangleShape dropZone(sf::Vector2f(160.f, 160.f));
    dropZone.setOrigin(80.f, 80.f); // Środek to połowa nowego wymiaru
    dropZone.setPosition(dropZonePos); // Teraz bierze pozycję z konkretnej mapy
    dropZone.setFillColor(sf::Color(0, 255, 0, 40));
    dropZone.setOutlineColor(sf::Color::Green);
    dropZone.setOutlineThickness(2.f);
    window.draw(dropZone);

    // --- RYSOWANIE BAZY ---
    if (!waypoints.empty()) {
        // Baza zawsze znajduje się na OSTATNIM punkcie ścieżki (waypoint)
        sf::Vector2f basePos = waypoints.back();

        sf::RectangleShape baseShape(sf::Vector2f(140.f, 140.f));
        baseShape.setOrigin(70.f, 70.f);
        baseShape.setPosition(basePos);

        // Ciemnoniebieski kolor bazy z jasną ramką
        baseShape.setFillColor(sf::Color(40, 60, 120));
        baseShape.setOutlineColor(sf::Color(100, 150, 255));
        baseShape.setOutlineThickness(3.f);

        window.draw(baseShape);
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
