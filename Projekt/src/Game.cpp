#include "Game.h"
#include "Config.h"
#include "zombie.h"


Game::Game() : window(sf::VideoMode(1280, 720), "DEAD ZONE") {
    window.setFramerateLimit(60);
    //map.loadFromFile("C:/Github/Projekt_Zaliczeniowy_TD/Projekt/assets/maps/map1.txt");
    map.loadFromFile("C:/Users/natal/OneDrive/Dokumenty/GitHub/Projekt_Zaliczeniowy_TD/Projekt/assets/maps/map1.txt");
}

void Game::processEvents(){
    sf::Event event;
    while (window.pollEvent(event)) {

        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::checkCollisions(){

}

void Game::update(float dt){
    checkCollisions();

    // === SYSTEM SPAWNOWANIA ZOMBIE ===
    spawnTimer += dt; // Zwiększamy licznik o czas, jaki minął od ostatniej klatki

    if (spawnTimer >= Config::SPAWN_INTERVAL) { //
        // Zabezpieczenie: Sprawdzamy, czy mapa w ogóle wczytała jakieś waypointy
        if (!map.getWaypoints().empty()) { //

            // Przekazujemy od razu całą listę punktów (waypointów) pobraną z mapy
            objects.push_back(std::make_unique<Walker>(map.getWaypoints()));
        }

        spawnTimer = 0.f; // Resetujemy licznik, żeby odliczał kolejne 1.5 sekundy
    }

    // Aktualizacja wszystkich obiektów w grze (wież, pocisków i zombie)
    for(auto& o:objects){ //
        o->update(dt, objects); //
    }

    // 3. Usuń martwe
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [](const std::unique_ptr<GameObject>& o) { return !o->isAlive(); }),
        objects.end()
        );
}

void Game::render() {
    window.clear(sf::Color::Black);
    map.draw(window);
    for(auto& o:objects){
        o->render(window);
    }
    // tu potem dojdzie rysowanie obiektów
    window.display();
}

void Game::run(){
    while(window.isOpen()){
        processEvents();
        sf::Time dt = clock.restart();
        update(dt.asSeconds());
        render();
    }
}
