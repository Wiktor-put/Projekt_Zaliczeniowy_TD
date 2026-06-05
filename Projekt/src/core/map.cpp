#include "map.h"
#include "Config.h"
#include "resourcemanager.h"
#include <cmath>

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

    // --- ŁADOWANIE TŁA MAPY PRZEZ MANAGER---
    const sf::Texture& tex = ResourceManager::getTexture(Config::Assets::BACKGROUND);
    bgSprite.setTexture(tex);

    // Skalujemy
    float scaleX = static_cast<float>(Config::WINDOW_WIDTH) / tex.getSize().x;
    float scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / tex.getSize().y;
    bgSprite.setScale(scaleX, scaleY);



}

void Map::draw(sf::RenderWindow& window) const {
    if (waypoints.size() < 2) return;

    // --- RYSOWANIE TŁA MAPY (Zawsze na samym spodzie) ---
    window.draw(bgSprite);

    // 2. RYSOWANIE ŚCIEŻKI Z TEKSTURĄ I ZAOKRĄGLONYMI ZAKRĘTAMI
    sf::Texture& pathTex = ResourceManager::getTexture(Config::Assets::PATH);
    pathTex.setRepeated(true); // Kluczowe! Pozwala teksturze zapętlać się na długich odcinkach

    float pathWidth = 50.f;             // Szerokość ścieżki
    float radius = pathWidth / 2.f;     // Promień dla "przegubów" na zakrętach

    // 2. RYSOWANIE ŚCIEŻKI
    for (size_t i = 0; i < waypoints.size() - 1; ++i) {
        sf::Vector2f p1 = waypoints[i];
        sf::Vector2f p2 = waypoints[i + 1];

        // --- RYSOWANIE OKRĄGŁEGO "PRZEGUBU" NA ZAKRĘCIE ---
        sf::CircleShape joint(radius);
        joint.setOrigin(radius, radius);
        joint.setPosition(p1);
        joint.setTexture(&pathTex);
        window.draw(joint);

        // --- RYSOWANIE PROSTEGO ODCINKA DROGI ---
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float length = std::sqrt(dx * dx + dy * dy);
        float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

        sf::RectangleShape segment(sf::Vector2f(length, pathWidth));
        segment.setOrigin(0.f, radius);
        segment.setPosition(p1);
        segment.setRotation(angle);
        segment.setTexture(&pathTex);

        // Zamiast rozciągać, mówimy SFML-owi, by kafelkował teksturę wzdłuż długości
        segment.setTextureRect(sf::IntRect(0, 0, static_cast<int>(length), static_cast<int>(pathWidth)));
        window.draw(segment);
    }
    // Dodanie ostatniego kółka na samym końcu ścieżki (w bazie)
    if (!waypoints.empty()) {
        sf::CircleShape lastJoint(radius);
        lastJoint.setOrigin(radius, radius);
        lastJoint.setPosition(waypoints.back());
        lastJoint.setTexture(&pathTex);
        window.draw(lastJoint);
    }

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
