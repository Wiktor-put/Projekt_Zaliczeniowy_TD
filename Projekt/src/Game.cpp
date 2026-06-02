#include "Game.h"
#include "Config.h"
#include "zombie.h"
#include "walker.h"
#include "runner.h"
#include "armored.h"
#include "tank.h"
#include "machineguntower.h"
#include "projectile.h"
#include "tower.h"
#include "snipertower.h"
#include "rockettower.h"
#include "flamethrowertower.h"
#include "slowertower.h"
#include "Random.h"
#include <cmath>

// ASSETS_DIR definiowane przez qmake jako $$PWD (katalog projektu).
// Fallback na "." gdy kompilowane bez qmake.
#ifndef ASSETS_DIR
#define ASSETS_DIR "."
#endif

Game::Game() : window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "DEAD ZONE"), state(GameState::PLAYING) {
    window.setFramerateLimit(Config::FPS_LIMIT);
    if (!font.loadFromFile(std::string(ASSETS_DIR) + "/assets/fonts/arial.ttf")) {
        std::cerr << "Nie udalo sie zaladowac fontu!" << std::endl;
    }

    startNewGame();
}

void Game::startNewGame() {
    objects.clear();           // usuń wszystkie zombi, wieże, pociski
    player.reset();            // reset gracza do startowych wartości
    map.loadFromFile(std::string(ASSETS_DIR) + "/assets/maps/map1.txt");  // przeładuj mapę (zwalnia sloty)
    spawnTimer = 0.f;
    selectedSlotIndex = -1;    // anuluj wybór slotu jeśli był
    state = GameState::PLAYING;
}

void Game::processEvents(){
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            // R restartuje grę po przegranej
            if (state == GameState::GAME_OVER && event.key.code == sf::Keyboard::R) {
                startNewGame();
                continue;
            }

            if (state != GameState::PLAYING) continue;
        }

        // Kliknięcie LPM
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                               static_cast<float>(event.mouseButton.y));

            // 1. Sprawdzamy, czy gracz nie kliknął w bonus
            bool clickedBonus = false;
            for (auto& obj : objects) {
                Bonus* bonus = dynamic_cast<Bonus*>(obj.get());
                if (bonus && bonus->isAlive() && bonus->contains(mouse)) {
                    applyBonus(bonus->getType()); // Nakładamy efekt (kasa/życie/EMP)
                    bonus->destroy(); // Zbieramy pudełko
                    clickedBonus = true;
                    break;
                }
            }

            // 2. Jeśli nie kliknął w bonus, zaznaczamy slot budowlany
            if (!clickedBonus) {
                int idx = map.getSlotAt(mouse);
                selectedSlotIndex = (idx >= 0 && !map.getSlots()[idx].occupied) ? idx : -1;
            }
        }

        // PPM — odznacz slot
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Right) {
            selectedSlotIndex = -1;
        }


        if (event.type == sf::Event::KeyPressed) {
            // Klawisz 1 — kup MachineGunTower w zaznaczonym slocie
            if (event.key.code == sf::Keyboard::Num1)
                tryBuyTower(1);

            // Klawisz 2 — kup SniperTower
            if (event.key.code == sf::Keyboard::Num2)
                tryBuyTower(2);

            if (event.key.code == sf::Keyboard::Num3)
                tryBuyTower(3);

            if (event.key.code == sf::Keyboard::Num4)
                tryBuyTower(4);
            if (event.key.code == sf::Keyboard::Num5)
                tryBuyTower(5);

            // Escape — odznacz slot bez kupowania
            if (event.key.code == sf::Keyboard::Escape)
                selectedSlotIndex = -1;
        }
    }
}

void Game::checkCollisions() {
    // Iteracja po wszystkich obiektach, szukamy pociski
    for (auto& obj : objects) {
        Projectile* projectile = dynamic_cast<Projectile*>(obj.get());
        if (!projectile || !projectile->isAlive()) continue;

        // Dla każdego pocisku - szukamy zombi w zasięgu kolizji
        for (auto& target : objects) {
            Zombie* zombie = dynamic_cast<Zombie*>(target.get());
            if (!zombie || !zombie->isAlive()) continue;

            // Sprawdzenie geometryczne (kwadrat odległości)
            sf::Vector2f diff = zombie->getPosition() - projectile->getPosition();
            float distSq = diff.x * diff.x + diff.y * diff.y;

            if (distSq < Config::HIT_RADIUS * Config::HIT_RADIUS) {
                // Polimorficzne wywołanie - Bullet/Rocket mają własną logikę
                projectile->onHit(zombie, objects);
                break;  // pocisk trafił, nie szukamy dalej (został zniszczony przez onHit)
            }
        }

        // Pocisk dotarł do zapamiętanej pozycji celu bez trafienia w nikogo - znika.
        // Sprawdzamy TUTAJ (po pętli zombi), nie w Projectile::update,
        // żeby kolizja miała szansę być wykryta zanim pocisk zniknie.
        if (projectile->isAlive() && projectile->hasReachedTarget()) {
            projectile->destroy();
        }
    }
}

void Game::update(float dt){
    if (state != GameState::PLAYING) return;
    spawnTimer += dt;

    if (spawnTimer >= Config::SPAWN_INTERVAL) {
        if (!map.getWaypoints().empty()) {
            objects.push_back(std::make_unique<Walker>(map.getWaypoints()));
            //objects.push_back(std::make_unique<Tank>(map.getWaypoints())); //teraz beda chodzic tanki
        }
        spawnTimer = 0.f;
    }

    // Ruch wszystkich obiektów PRZED sprawdzeniem kolizji.
    // Używamy indeksu (nie range-for) bo Tower::shoot() może dodać nowe obiekty
    // do wektora - push_back może realokować pamięć i unieważnić iteratory range-for.
    const size_t updateCount = objects.size();
    for (size_t i = 0; i < updateCount; ++i) {
        objects[i]->update(dt, objects);
    }

    checkCollisions();

    std::vector<std::unique_ptr<GameObject>> newBonuses; // Tymczasowa lista chroniąca pamięć

    // W Game::update, po checkCollisions, przed usunięciem martwych
    for (auto& obj : objects) {
        if (obj->isAlive()) continue;  // tylko martwe

        Zombie* zombie = dynamic_cast<Zombie*>(obj.get());
        if (zombie && !zombie->reachedEnd()) {  // zginął OD pocisku, nie doszedł do bazy
            player.addMoney(zombie->getReward());
            player.addScore(zombie->getReward());

            // SZANSA NA DROP BONUSU
            if (Random::chance(Config::BONUS_DROP_CHANCE)) {
                BonusType randomType = static_cast<BonusType>(Random::intInRange(0, 2));

                // Pobieramy pozycję strefy z mapy
                sf::Vector2f dzPos = map.getDropZonePos();

                // Dodajemy mały losowy "rozrzut" na osi X, żeby paczki nie lądowały idealnie jedna na drugiej
                float randomOffsetX = Random::floatInRange(-60.f, 60.f);
                float randomOffsetY = Random::floatInRange(-60.f, 60.f);

                // Startujemy nad strefą zrzutu
                sf::Vector2f startPos(dzPos.x + randomOffsetX, -50.f);
                float targetY = dzPos.y + randomOffsetY;



                newBonuses.push_back(std::make_unique<Bonus>(startPos, targetY, randomType));
            }
        }
        else if(zombie && zombie->reachedEnd()){
            player.lostLives(zombie->getLifeCost());
        }
    }

    // Fizyczne wrzucenie pudełek na mapę
    for (auto& b : newBonuses) {
        objects.push_back(std::move(b));
    }

    if (!player.isAlive()) {
        state = GameState::GAME_OVER;
        std::cout << "GAME OVER! Wynik: " << player.getScore() << std::endl;
    }

    removeDeadObjects();
}

void Game::renderGameOver() {
    // Półprzezroczyste czarne tło zaciemniające grę
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(Config::WINDOW_WIDTH),
        static_cast<float>(Config::WINDOW_HEIGHT)
        ));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));  // czarny, 70% widoczność
    window.draw(overlay);

    if (font.getInfo().family.empty()) return;  // brak fontu - tylko czarne tło

    // Napis "GAME OVER"
    sf::Text gameOverText("GAME OVER", font, 80);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    gameOverText.setPosition(
        Config::WINDOW_WIDTH / 2.f,
        Config::WINDOW_HEIGHT / 2.f - 60.f
        );
    window.draw(gameOverText);

    // Wynik
    sf::Text scoreText("Wynik: " + std::to_string(player.getScore()), font, 30);
    scoreText.setFillColor(sf::Color::White);
    bounds = scoreText.getLocalBounds();
    scoreText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    scoreText.setPosition(
        Config::WINDOW_WIDTH / 2.f,
        Config::WINDOW_HEIGHT / 2.f + 20.f
        );
    window.draw(scoreText);

    // Instrukcja
    sf::Text restartText("Wcisnij R aby zagrac ponownie", font, 24);
    restartText.setFillColor(sf::Color::White);
    bounds = restartText.getLocalBounds();
    restartText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    restartText.setPosition(
        Config::WINDOW_WIDTH / 2.f,
        Config::WINDOW_HEIGHT / 2.f + 80.f
        );
    window.draw(restartText);
}


void Game::renderUI() {
    // Zabezpieczenie: jeśli czcionka się nie wczytała, nie rysujemy napisów
    if (font.getInfo().family.empty()) return;

    // 1. Wyświetlanie pieniędzy
    sf::Text moneyText("Kasa: $" + std::to_string(player.getMoney()), font, 24);
    moneyText.setPosition(20.f, 20.f);
    moneyText.setFillColor(sf::Color::Yellow);

    // 2. Wyświetlanie żyć
    sf::Text livesText("Zycia: " + std::to_string(player.getLives()), font, 24);
    livesText.setPosition(20.f, 50.f);
    livesText.setFillColor(sf::Color::Red);

    // 3. Wyświetlanie wyniku punktowego
    sf::Text scoreText("Punkty: " + std::to_string(player.getScore()), font, 24);
    scoreText.setPosition(20.f, 80.f);
    scoreText.setFillColor(sf::Color::White);

    // Rysujemy bazowe statystyki
    window.draw(moneyText);
    window.draw(livesText);
    window.draw(scoreText);

    // 4. Wyświetlanie menu budowania TYLKO gdy zaznaczono slot
    if (selectedSlotIndex >= 0) {
        sf::Text buildText(
            "Wybierz wieze:\n"
            "[1] Karabin ($50)\n"
            "[2] Snajper ($100)\n"
            "[3] Wyrzutnia ($150)\n"
            "[4] Miotacz Ognia ($80)\n"
            "[5] Wieza Spowalniajaca ($60)\n"
            "[ESC] Anuluj", font, 20);

        buildText.setPosition(20.f, 130.f);
        buildText.setFillColor(sf::Color::Cyan);
        window.draw(buildText);


        // --- NAPIS NA DROP ZONE ---
        sf::Text dzText("DROP ZONE", font, 16);
        dzText.setFillColor(sf::Color(0, 255, 0, 150));

        // Pobieramy wymiary tekstu, by ustawić jego środek (Origin)
        // Dzięki temu tekst będzie idealnie na środku kwadratu niezależnie od długości słowa
        sf::FloatRect bounds = dzText.getLocalBounds();
        dzText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sf::Vector2f dzPos = map.getDropZonePos();
        dzText.setPosition(dzPos.x, dzPos.y); // Wyśrodkowany tekst w strefie konkretnej mapy

        window.draw(dzText);
    }
}


void Game::render() {
    window.clear(sf::Color::Black);
    map.draw(window);

    // Rysowanie strefy zrzutu (Drop Zone)
    sf::RectangleShape dropZone(sf::Vector2f(100.f, 100.f));
    dropZone.setOrigin(50.f, 50.f);
    //dropZone.setPosition(Config::DROP_ZONE_X, Config::DROP_ZONE_Y);
    dropZone.setFillColor(sf::Color(0, 255, 0, 40)); // Półprzezroczysty zielony
    dropZone.setOutlineColor(sf::Color::Green);
    dropZone.setOutlineThickness(2.f);
    window.draw(dropZone);

    // napis na strefie
    if (!font.getInfo().family.empty()) {
        sf::Text dzText("DROP\nZONE", font, 14);
        dzText.setFillColor(sf::Color(0, 255, 0, 150));
        //dzText.setPosition(Config::DROP_ZONE_X - 35.f, Config::DROP_ZONE_Y - 15.f);
        window.draw(dzText);
    }

    for (auto& o : objects) {
        o->render(window);
    }

    // Podświetlenie zaznaczonego slotu (żółta ramka)
    if (selectedSlotIndex >= 0) {
        const TowerSlot& slot = map.getSlots()[selectedSlotIndex];
        sf::RectangleShape highlight(sf::Vector2f(34.f, 34.f));
        highlight.setOrigin(17.f, 17.f);
        highlight.setPosition(slot.position);
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(2.f);
        window.draw(highlight);
    }

    if (state == GameState::PLAYING) {
        renderUI();
    }

    if (state == GameState::GAME_OVER) {
        renderGameOver();
    }

    window.display();
}

void Game::tryBuyTower(int type) {
    if (selectedSlotIndex < 0) return;
    TowerSlot& slot = map.getSlots()[selectedSlotIndex];
    if (slot.occupied) return;

    int cost = 0;
    if (type == 1) cost = Config::MachineGunTower::COST;
    else if (type == 2) cost = Config::SniperTower::COST; // Koszt snajpera
    else if (type == 3) cost = Config::RocketTower::COST; // Koszt rakiet
    else if (type == 4) cost = Config::FlamethrowerTower::COST;
    else if (type == 5) cost = Config::SlowerTower::COST;

    if (!player.spendMoney(cost)) return;

    map.occupiedSlot(slot);

    if (type == 1) {
        objects.push_back(std::make_unique<MachineGunTower>(slot.position)); //
    }
    else if (type == 2) {
        objects.push_back(std::make_unique<SniperTower>(slot.position));
    }
    else if (type == 3) {
        objects.push_back(std::make_unique<RocketTower>(slot.position));
    }
    else if (type == 4) objects.push_back(std::make_unique<FlamethrowerTower>(slot.position));
    else if (type == 5) objects.push_back(std::make_unique<SlowerTower>(slot.position));

    selectedSlotIndex = -1;
}

void Game::removeDeadObjects() {
    // Zeruj martwe targety wież zanim zwolnimy pamięć (dangling pointer fix).
    // Tower::currentTarget to surowy wskaźnik — musi być nullptr zanim unique_ptr zombi zginie.
    for (auto& obj : objects) {
        Tower* tower = dynamic_cast<Tower*>(obj.get());
        if (tower) tower->clearDeadTarget();
    }
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [](const std::unique_ptr<GameObject>& o) { return !o->isAlive(); }),
        objects.end()
    );
}

void Game::run() {
    while (window.isOpen()) {           // ← zawsze chodzi
        processEvents();
        sf::Time dt = clock.restart();
        update(dt.asSeconds());
        render();
    }
}

void Game::applyBonus(BonusType type) {
    if (type == BonusType::AMMO) {
        player.addMoney(Config::AMMO_BONUS_VALUE); // +30 dolarów
    } else if (type == BonusType::MEDKIT) {
        player.addLives(Config::MEDKIT_BONUS_VALUE); // +1 życie
    } else if (type == BonusType::EMP) {
        // Granat EMP ogłusza KAŻDEGO zombiaka na całej mapie!
        for (auto& obj : objects) {
            Zombie* z = dynamic_cast<Zombie*>(obj.get());
            if (z && z->isAlive()) {
                z->applyStun(Config::EMP_STUN_DURATION);
            }
        }
    }
}