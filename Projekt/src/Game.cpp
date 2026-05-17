#include "Game.h"


Game::Game() : window(sf::VideoMode(1280, 720), "DEAD ZONE") {
    window.setFramerateLimit(60);
}

void Game::processEvents(){
    sf::Event event;
    while (window.pollEvent(event)) {

        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
    }
}
void Game::update(float dt){
    for(auto& o:objects){
        o->update(dt);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    for(auto& o:objects){
        o->render(window);
    }
    // tu potem dojdzie rysowanie obiektów
    window.display();
}

void Game::checkCollisions(){}

void Game::run(){
    while(window.isOpen()){
        processEvents();
        sf::Time dt = clock.restart();
        update(dt.asSeconds());
        render();
    }
}
