#include "Game.h"
#include "Config.h"
#include "walker.h"

// ASSETS_DIR definiowane przez qmake jako $$PWD (katalog projektu).
// Fallback na "." gdy kompilowane bez qmake.
#ifndef ASSETS_DIR
#define ASSETS_DIR "."
#endif

Game::Game() : window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "DEAD ZONE") {
    window.setFramerateLimit(Config::FPS_LIMIT);
    map.loadFromFile(std::string(ASSETS_DIR) + "/assets/maps/map1.txt");
}

void Game::processEvents(){
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::checkCollisions(){

}

void Game::update(float dt){
    checkCollisions();

    spawnTimer += dt;

    if (spawnTimer >= Config::SPAWN_INTERVAL) {
        if (!map.getWaypoints().empty()) {
            objects.push_back(std::make_unique<Walker>(map.getWaypoints()));
        }
        spawnTimer = 0.f;
    }

    for (auto& o : objects) {
        o->update(dt, objects);
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [](const std::unique_ptr<GameObject>& o) { return !o->isAlive(); }),
        objects.end()
    );
}

void Game::render() {
    window.clear(sf::Color::Black);
    map.draw(window);
    for (auto& o : objects) {
        o->render(window);
    }
    window.display();
}

void Game::run(){
    while (window.isOpen()) {
        processEvents();
        sf::Time dt = clock.restart();
        update(dt.asSeconds());
        render();
    }
}
