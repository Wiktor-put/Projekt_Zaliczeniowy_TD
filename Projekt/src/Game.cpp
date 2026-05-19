#include "Game.h"


Game::Game() : window(sf::VideoMode(1280, 720), "DEAD ZONE") {
    window.setFramerateLimit(60);
    map.loadFromFile("C:/Github/Projekt_Zaliczeniowy_TD/Projekt/build/Desktop_Qt_6_10_2_MinGW_64_bit-Debug/assets/maps/map1.txt");
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

    for(auto& o:objects){
        o->update(dt, objects);
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
