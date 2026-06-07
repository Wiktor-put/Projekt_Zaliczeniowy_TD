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
        iss >> type;

        if (type == 'B') {
            // Tło tej mapy: reszta linii to ścieżka do pliku (może zawierać spacje).
            std::string rest;
            std::getline(iss, rest);
            // usuń wiodące białe znaki
            size_t start = rest.find_first_not_of(" \t");
            if (start != std::string::npos) backgroundPath = rest.substr(start);
            continue;
        }

        float x, y;
        if (iss >> x >> y) {
            if (type == 'W') {
                waypoints.push_back(sf::Vector2f(x, y));
            } else if (type == 'S') {
                towerSlots.push_back({sf::Vector2f(x, y), false});
            } else if (type == 'D') {
                dropZonePos = sf::Vector2f(x, y);
            } else {
                std::cerr << "Nieznany typ '" << type << "' w linii " << lineNumber << std::endl;
            }
        } else {
            std::cerr << "Bledna linia " << lineNumber << ": " << line << std::endl;
        }
    }

    std::cout << "Wczytano " << waypoints.size() << " waypointow i "
              << towerSlots.size() << " slotow z " << path << std::endl;

    // Cała kosztowna praca (ładowanie tekstur, budowa geometrii) — raz, tutaj.
    buildVisuals();
}

void Map::buildVisuals() {
    // --- TŁO MAPY (własne dla każdej planszy, przeskalowane do okna) ---
    const sf::Texture& bgTex = ResourceManager::getTexture(backgroundPath);
    bgSprite.setTexture(bgTex, true);
    bgSprite.setScale(static_cast<float>(Config::WINDOW_WIDTH) / bgTex.getSize().x,
                      static_cast<float>(Config::WINDOW_HEIGHT) / bgTex.getSize().y);

    // --- GEOMETRIA ŚCIEŻKI (przeguby na zakrętach + proste odcinki) ---
    pathJoints.clear();
    pathSegments.clear();

    if (waypoints.size() >= 2) {
        sf::Texture& pathTex = ResourceManager::getTexture(Config::Assets::PATH);
        pathTex.setRepeated(true); // pozwala kafelkować teksturę na długich odcinkach

        const float pathWidth = 50.f;
        const float radius = pathWidth / 2.f;

        auto makeJoint = [&](sf::Vector2f pos) {
            sf::CircleShape joint(radius);
            joint.setOrigin(radius, radius);
            joint.setPosition(pos);
            joint.setTexture(&pathTex);
            pathJoints.push_back(joint);
        };

        for (size_t i = 0; i + 1 < waypoints.size(); ++i) {
            sf::Vector2f p1 = waypoints[i];
            sf::Vector2f p2 = waypoints[i + 1];
            makeJoint(p1);

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            float length = std::sqrt(dx * dx + dy * dy);
            float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

            sf::RectangleShape segment(sf::Vector2f(length, pathWidth));
            segment.setOrigin(0.f, radius);
            segment.setPosition(p1);
            segment.setRotation(angle);
            segment.setTexture(&pathTex);
            // Kafelkowanie tekstury wzdłuż odcinka (zamiast rozciągania).
            segment.setTextureRect(sf::IntRect(0, 0, static_cast<int>(length), static_cast<int>(pathWidth)));
            pathSegments.push_back(segment);
        }
        makeJoint(waypoints.back()); // domknięcie ścieżki w bazie
    }

    // --- DROP ZONE ---
    sf::Texture& dzTex = ResourceManager::getTexture(Config::Assets::DROPZONE);
    dzSprite.setTexture(dzTex, true);
    dzSprite.setOrigin(dzTex.getSize().x / 2.f, dzTex.getSize().y / 2.f);
    dzSprite.setScale(4.8f, 4.8f);
    dzSprite.setPosition(dropZonePos);

    // --- BRAMA / ŚCIANY BAZY ---
    gateSprites.clear();
    if (!waypoints.empty()) {
        const float scale = 3.5f;
        sf::Texture& gateTex = ResourceManager::getTexture(Config::Assets::BASE_GATE);
        sf::Texture& vertTex = ResourceManager::getTexture(Config::Assets::BASE_WALL_VERT);

        sf::Vector2f basePos = waypoints.back();
        const float offset = 55.f;
        const float wallWidth = gateTex.getSize().x * scale;
        const float leftX = basePos.x - wallWidth - (wallWidth / 2.f);
        const float gapOffset = -3.5f;

        auto makeSprite = [&](sf::Texture& tex, sf::Vector2f pos) {
            sf::Sprite s(tex);
            s.setOrigin(tex.getSize().x / 2.f, tex.getSize().y / 2.f);
            s.setScale(scale, scale);
            s.setPosition(pos);
            gateSprites.push_back(s);
        };

        // Najpierw pionowe ściany (na spodzie), potem poziome przykrywające ich końce.
        makeSprite(vertTex, {leftX, basePos.y - offset - gapOffset});
        makeSprite(vertTex, {leftX, basePos.y + offset + gapOffset});
        makeSprite(gateTex, {basePos.x, basePos.y - offset});
        makeSprite(gateTex, {basePos.x, basePos.y + offset});
        makeSprite(gateTex, {basePos.x - wallWidth, basePos.y - offset});
        makeSprite(gateTex, {basePos.x - wallWidth, basePos.y + offset});
    }
}

void Map::draw(sf::RenderWindow& window) const {
    // Rysujemy wyłącznie gotowe obiekty zbudowane w buildVisuals() — bez alokacji
    // i bez lookupów tekstur w trakcie klatki.
    window.draw(bgSprite);

    for (const auto& joint : pathJoints) window.draw(joint);
    for (const auto& segment : pathSegments) window.draw(segment);

    // Sloty są lekkie i mogą zmieniać wygląd (np. podświetlenie) — rysujemy na bieżąco.
    for (const auto& slot : towerSlots) {
        sf::RectangleShape shape(sf::Vector2f(30.f, 30.f));
        shape.setPosition(slot.position - sf::Vector2f(15.f, 15.f));
        shape.setFillColor(sf::Color(60, 60, 60, 100));
        shape.setOutlineColor(sf::Color(120, 120, 120));
        shape.setOutlineThickness(1.f);
        window.draw(shape);
    }

    window.draw(dzSprite);
    for (const auto& g : gateSprites) window.draw(g);
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
