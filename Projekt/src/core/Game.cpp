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
#include "rocket.h"
#include "snipertower.h"
#include "rockettower.h"
#include "flamethrowertower.h"
#include "slowertower.h"
#include "Random.h"
#include "resourcemanager.h"
#include <cmath>
#include <fstream>

// ASSETS_DIR definiowane przez qmake jako $$PWD (katalog projektu).
// Fallback na "." gdy kompilowane bez qmake.
#ifndef ASSETS_DIR
#define ASSETS_DIR "."
#endif

Game::Game() : window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), "DEAD ZONE"), state(GameState::MENU), currentMapPath(Config::Maps::PATHS[0]), menuSelectedOption(0) {
    window.setFramerateLimit(Config::FPS_LIMIT);
    if (!font.loadFromFile(std::string(ASSETS_DIR) + "/assets/fonts/arial.ttf")) {
        std::cerr << "Nie udalo sie zaladowac fontu!" << std::endl;
    }
    initButtons();  // przyciski wymagają wczytanej czcionki
}

void Game::initButtons() {
    const float cx = Config::WINDOW_WIDTH / 2.f;  // środek poziomy okna
    const sf::Vector2f bigSize(360.f, 50.f);      // standardowy rozmiar przycisku menu

    // --- MENU GŁÓWNE ---
    const char* menuLabels[4] = { "Nowa gra", "Wczytaj gre", "Wyniki", "Wyjdz" };
    float y = 360.f;
    for (int i = 0; i < 4; ++i) {
        menuButtons[i].setup(font, menuLabels[i], {cx, y}, bigSize, 30);
        y += 65.f;
    }

    // --- WYBÓR MAPY (jeden przycisk na mapę z Config) ---
    mapButtons.resize(Config::Maps::COUNT);
    y = 320.f;
    for (int i = 0; i < Config::Maps::COUNT; ++i) {
        mapButtons[i].setup(font, Config::Maps::NAMES[i], {cx, y}, bigSize, 32);
        y += 65.f;
    }

    // --- PAUZA ---
    pauseButtons[0].setup(font, "Wznow (ESC)", {cx, Config::WINDOW_HEIGHT / 2.f + 40.f}, bigSize, 28);
    pauseButtons[1].setup(font, "Menu glowne (M)", {cx, Config::WINDOW_HEIGHT / 2.f + 105.f}, bigSize, 28);

    // --- GAME OVER ---
    gameOverButtons[0].setup(font, "Zagraj ponownie (R)", {cx, Config::WINDOW_HEIGHT / 2.f + 80.f}, bigSize, 28);
    gameOverButtons[1].setup(font, "Menu glowne (M)", {cx, Config::WINDOW_HEIGHT / 2.f + 145.f}, bigSize, 28);

    // --- EKRAN WYNIKÓW ---
    hsBackButton.setup(font, "Powrot do menu (ESC)", {cx, Config::WINDOW_HEIGHT - 60.f}, {320.f, 46.f}, 22);

    // --- HUD: panel budowy/ulepszeń (lewy górny róg, środek panelu x=150) ---
    const float panelCx = 150.f;
    const sf::Vector2f hudSize(260.f, 28.f);
    const char* buildLabels[5] = {
        "1: Karabin ($50)",
        "2: Snajper ($100)",
        "3: Wyrzutnia ($150)",
        "4: Miotacz ognia ($80)",
        "5: Spowalniacz ($60)"
    };
    float hy = 120.f;
    for (int i = 0; i < 5; ++i) {
        buildButtons[i].setup(font, buildLabels[i], {panelCx, hy}, hudSize, 16);
        hy += 34.f;
    }
    // Teksty ulepszenia/sprzedaży są dynamiczne — ustawiamy je co klatkę w renderUI.
    upgradeButton.setup(font, "", {panelCx, 130.f}, {260.f, 34.f}, 16);
    sellButton.setup(font, "", {panelCx, 175.f}, {260.f, 34.f}, 16);
}

void Game::startNewGame() {
    objects.clear();           // usuń wszystkie zombi, wieże, pociski
    player.reset();            // reset gracza do startowych wartości
    map.loadFromFile(std::string(ASSETS_DIR) + currentMapPath);  // przeładuj wybraną mapę (zwalnia sloty)
    spawnTimer = 0.f;
    selectedSlotIndex = -1;    // anuluj wybór slotu jeśli był
    state = GameState::PLAYING;
    waveManager.reset();
    waveManager.loadFromFile(std::string(ASSETS_DIR) + "/assets/waves/waves_testowe.txt");
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {

        // 1. ZAMKNIĘCIE OKNA
        case sf::Event::Closed:
            window.close();
            break;

        // 2. OBSŁUGA MYSZY — RUCH (podświetlanie przycisków pod kursorem)
        case sf::Event::MouseMoved: {
            sf::Vector2f m(static_cast<float>(event.mouseMove.x),
                           static_cast<float>(event.mouseMove.y));
            // W menu i wyborze mapy mysz współdzieli zaznaczenie z klawiaturą.
            if (state == GameState::MENU) {
                for (int i = 0; i < 4; ++i)
                    if (menuButtons[i].contains(m)) menuSelectedOption = i;
            } else if (state == GameState::MAP_SELECT) {
                for (int i = 0; i < Config::Maps::COUNT; ++i)
                    if (mapButtons[i].contains(m)) mapSelectedOption = i;
            }
            break;
        }

        // 2b. OBSŁUGA MYSZY — KLIKNIĘCIE
        case sf::Event::MouseButtonPressed: {
            sf::Vector2f mouse(static_cast<float>(event.mouseButton.x),
                               static_cast<float>(event.mouseButton.y));
            bool left = (event.mouseButton.button == sf::Mouse::Left);

            switch (state) {
            case GameState::MENU:
                if (left)
                    for (int i = 0; i < 4; ++i)
                        if (menuButtons[i].contains(mouse)) { handleMenuChoice(i); break; }
                break;

            case GameState::MAP_SELECT:
                if (left)
                    for (int i = 0; i < Config::Maps::COUNT; ++i)
                        if (mapButtons[i].contains(mouse)) {
                            currentMapPath = Config::Maps::PATHS[i];
                            startNewGame();
                            break;
                        }
                break;

            case GameState::PAUSED:
                if (left) {
                    if (pauseButtons[0].contains(mouse)) state = GameState::PLAYING;
                    else if (pauseButtons[1].contains(mouse)) { state = GameState::MENU; menuSelectedOption = 0; }
                }
                break;

            case GameState::GAME_OVER:
                if (left) {
                    if (gameOverButtons[0].contains(mouse)) startNewGame();
                    else if (gameOverButtons[1].contains(mouse)) { state = GameState::MENU; menuSelectedOption = 0; }
                }
                break;

            case GameState::HIGHSCORES:
                if (left && hsBackButton.contains(mouse)) { state = GameState::MENU; menuSelectedOption = 0; }
                break;

            case GameState::PLAYING:
                if (left) {
                    // Najpierw przyciski HUD aktywnego slotu — żeby klik w panel
                    // nie odznaczał slotu (panel nie leży nad slotem mapy).
                    if (selectedSlotIndex >= 0) {
                        const TowerSlot& slot = map.getSlots()[selectedSlotIndex];
                        bool handled = false;
                        if (!slot.occupied) {
                            for (int i = 0; i < 5; ++i)
                                if (buildButtons[i].contains(mouse)) { tryBuyTower(i + 1); handled = true; break; }
                        } else {
                            if (upgradeButton.contains(mouse)) { tryUpgradeTower(); handled = true; }
                            else if (sellButton.contains(mouse)) { trySellTower(); handled = true; }
                        }
                        if (handled) break;
                    }

                    // Kliknięcie w bonus (paczkę) na mapie.
                    for (auto& obj : objects) {
                        Bonus* bonus = dynamic_cast<Bonus*>(obj.get());
                        if (bonus && bonus->isAlive() && bonus->contains(mouse)) {
                            applyBonus(bonus->getType());
                            bonus->destroy();
                            break;
                        }
                    }

                    // Zaznaczenie slotu pod kursorem (-1 gdy kliknięto poza slotem).
                    int idx = map.getSlotAt(mouse);
                    selectedSlotIndex = (idx >= 0) ? idx : -1;

                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    selectedSlotIndex = -1; // PPM — odznacz slot
                }
                break;

            default:
                break;
            }
            break;
        }

        // 3. OBSŁUGA KLAWIATURY
        case sf::Event::KeyPressed:
            // Dzielimy logikę klawiszy w zależności od obecnego stanu gry
            switch (state) {
            case GameState::MENU:
                if (event.key.code == sf::Keyboard::Up) {
                    menuSelectedOption = (menuSelectedOption > 0) ? menuSelectedOption - 1 : 3;
                } else if (event.key.code == sf::Keyboard::Down) {
                    menuSelectedOption = (menuSelectedOption < 3) ? menuSelectedOption + 1 : 0;
                } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
                    handleMenuChoice(menuSelectedOption);
                }
                break;

            case GameState::MAP_SELECT:
                if (event.key.code == sf::Keyboard::Up) {
                    mapSelectedOption = (mapSelectedOption > 0) ? mapSelectedOption - 1 : Config::Maps::COUNT - 1;
                } else if (event.key.code == sf::Keyboard::Down) {
                    mapSelectedOption = (mapSelectedOption < Config::Maps::COUNT - 1) ? mapSelectedOption + 1 : 0;
                } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
                    currentMapPath = Config::Maps::PATHS[mapSelectedOption];  // zapamiętaj wybór
                    startNewGame();                                           // ustawia stan na PLAYING
                } else if (event.key.code == sf::Keyboard::Escape) {
                    state = GameState::MENU;  // powrót do menu głównego
                }
                break;

            case GameState::GAME_OVER:
                if (event.key.code == sf::Keyboard::R) {
                    startNewGame();  // R — zagraj ponownie na tej samej mapie
                } else if (event.key.code == sf::Keyboard::M) {
                    state = GameState::MENU;  // M — powrot do menu glownego
                    menuSelectedOption = 0;
                }
                break;

            case GameState::HIGHSCORES:
                // Z tablicy wynikow wracamy do menu (ESC lub M).
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::M) {
                    state = GameState::MENU;
                    menuSelectedOption = 0;
                }
                break;

            case GameState::PAUSED:
                if (event.key.code == sf::Keyboard::Escape) {
                    state = GameState::PLAYING;
                    std::cout << "Wznowiono\n";
                } else if (event.key.code == sf::Keyboard::M) {
                    state = GameState::MENU;
                    menuSelectedOption = 0;
                    std::cout << "Powrot do menu\n";
                }
                break;

            case GameState::PLAYING:
                // Skoro jesteśmy w PLAYING, używamy switcha dla konkretnych klawiszy
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    if (selectedSlotIndex != -1) {
                        selectedSlotIndex = -1;
                        std::cout << "Anulowano wybor slotu\n";
                    } else {
                        state = GameState::PAUSED;
                        std::cout << "PAUZA\n";
                    }
                    break;
                case sf::Keyboard::Num1: tryBuyTower(1); break;
                case sf::Keyboard::Num2: tryBuyTower(2); break;
                case sf::Keyboard::Num3: tryBuyTower(3); break;
                case sf::Keyboard::Num4: tryBuyTower(4); break;
                case sf::Keyboard::Num5: tryBuyTower(5); break;
                case sf::Keyboard::U:    tryUpgradeTower(); break;
                case sf::Keyboard::S:    trySellTower(); break;
                case sf::Keyboard::F5:   saveGame(); break;
                case sf::Keyboard::F9:   loadGame(); break;
                case sf::Keyboard::Space:
                    if (!waveManager.isWaveInProgress()) {
                        waveManager.startNextWave();
                    }
                    break;
                default:
                    break;
                }
                break; // Koniec GameState::PLAYING
            }
            break; // Koniec sf::Event::KeyPressed

        default:
            break;
        }
    }
}

void Game::handleMenuChoice(int option) {
    switch (option) {
    case 0:  // Nowa gra — najpierw wybór mapy
        mapSelectedOption = 0;
        state = GameState::MAP_SELECT;
        break;

    case 1:  // Wczytaj gre
        // Implementacja w MS4 fazie 3
        std::cout << "Wczytywanie - dostepne w MS4 fazie 3" << std::endl;
        break;

    case 2:  // Wyniki
        state = GameState::HIGHSCORES;
        break;

    case 3:  // Wyjdz
        window.close();
        break;
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
            Rocket* roc = dynamic_cast<Rocket*>(projectile);
            if(roc && roc->isAlive())
                roc->explosion(objects);
            projectile->destroy();
        }
    }
}

void Game::update(float dt) {
    switch (state) {
    case GameState::MENU:
        // Menu jest statyczne, nic nie aktualizujemy
        break;

    case GameState::MAP_SELECT:
        // Ekran wyboru mapy jest statyczny
        break;

    case GameState::PLAYING:
        updatePlaying(dt);
        break;

    case GameState::PAUSED:
        // Pauza - gra zatrzymana, ekran statyczny
        break;

    case GameState::GAME_OVER:
        // Ekran konca, nic nie aktualizujemy
        break;

    case GameState::HIGHSCORES:
        // Tablica wynikow, ekran statyczny
        break;
    }
}

void Game::updatePlaying(float dt){
    if (state != GameState::PLAYING) return;
    waveManager.update(dt, objects, map.getWaypoints());

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

void Game::renderMenu() {
    // --- RYSOWANIE TŁA MENU GŁÓWNEGO ---
    sf::Texture& menuTex = ResourceManager::getTexture(Config::Assets::MAIN_MENU_BG);
    sf::Sprite menuSprite(menuTex);

    // Skalujemy obrazek tak, aby idealnie wypełnił całe okno gry
    float scaleX = static_cast<float>(Config::WINDOW_WIDTH) / menuTex.getSize().x;
    float scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / menuTex.getSize().y;
    menuSprite.setScale(scaleX, scaleY);

    // Rysujemy tło
    window.draw(menuSprite);
    /*// Tlo - ciemny gradient (na razie zwykly kolor)
    sf::RectangleShape bg(sf::Vector2f(
        static_cast<float>(Config::WINDOW_WIDTH),
        static_cast<float>(Config::WINDOW_HEIGHT)
        ));
    bg.setFillColor(sf::Color(20, 30, 20));  // ciemny zielony
    window.draw(bg);*/

    if (font.getInfo().family.empty()) return;

    // Tytul gry
    sf::Text title("DEAD ZONE", font, 80);
    title.setFillColor(sf::Color(150, 255, 100));  // toksyczna zielen
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3.f);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    title.setPosition(Config::WINDOW_WIDTH / 2.f, 150.f);
    window.draw(title);

    // Podtytul
    sf::Text subtitle("Tower Defense | Zombie Apocalypse", font, 22);
    subtitle.setFillColor(sf::Color::Black);
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setOrigin(subBounds.width / 2.f, subBounds.height / 2.f);
    subtitle.setPosition(Config::WINDOW_WIDTH / 2.f, 230.f);
    window.draw(subtitle);

    // Opcje menu jako przyciski (klasa Button). Podświetlenie steruje wspólnym
    // indeksem menuSelectedOption — ustawianym i przez klawiaturę, i przez mysz.
    for (int i = 0; i < 4; ++i)
        menuButtons[i].draw(window, i == menuSelectedOption);

    // Instrukcja na dole
    sf::Text hint("Strzalki + ENTER lub mysz, aby wybrac", font, 16);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setOrigin(hintBounds.width / 2.f, hintBounds.height / 2.f);
    hint.setPosition(Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT - 40.f);
    window.draw(hint);
}

void Game::renderMapSelect() {
    // Tło reużywamy z menu głównego dla spójności wizualnej.
    sf::Texture& menuTex = ResourceManager::getTexture(Config::Assets::MAIN_MENU_BG);
    sf::Sprite menuSprite(menuTex);
    menuSprite.setScale(static_cast<float>(Config::WINDOW_WIDTH) / menuTex.getSize().x,
                        static_cast<float>(Config::WINDOW_HEIGHT) / menuTex.getSize().y);
    window.draw(menuSprite);

    if (font.getInfo().family.empty()) return;

    // Nagłówek
    sf::Text title("WYBIERZ MAPE", font, 60);
    title.setFillColor(sf::Color(150, 255, 100));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3.f);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    title.setPosition(Config::WINDOW_WIDTH / 2.f, 150.f);
    window.draw(title);

    // Lista dostępnych map jako przyciski (klasa Button).
    for (int i = 0; i < Config::Maps::COUNT; ++i)
        mapButtons[i].draw(window, i == mapSelectedOption);

    // Instrukcja na dole
    sf::Text hint("Strzalki + ENTER lub mysz, aby grac; ESC aby wrocic", font, 16);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setOrigin(hintBounds.width / 2.f, hintBounds.height / 2.f);
    hint.setPosition(Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT - 40.f);
    window.draw(hint);
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

    // Przyciski (klasa Button) — klawisze R/M nadal działają, mysz podświetla.
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    gameOverButtons[0].draw(window, gameOverButtons[0].contains(m));
    gameOverButtons[1].draw(window, gameOverButtons[1].contains(m));
}

void Game::renderPaused() {
    // Polprzezroczyste tlo zaciemniajace rozgrywke
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(Config::WINDOW_WIDTH),
        static_cast<float>(Config::WINDOW_HEIGHT)
        ));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));  // 60% widocznosc
    window.draw(overlay);

    if (font.getInfo().family.empty()) return;

    // Napis PAUZA
    sf::Text pauseText("PAUZA", font, 80);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOutlineColor(sf::Color(150, 255, 100));
    pauseText.setOutlineThickness(3.f);
    sf::FloatRect bounds = pauseText.getLocalBounds();
    pauseText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    pauseText.setPosition(
        Config::WINDOW_WIDTH / 2.f,
        Config::WINDOW_HEIGHT / 2.f - 50.f
        );
    window.draw(pauseText);

    // Przyciski (klasa Button) — podświetlane pod kursorem myszy.
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    pauseButtons[0].draw(window, pauseButtons[0].contains(m));
    pauseButtons[1].draw(window, pauseButtons[1].contains(m));
}

void Game::renderHighscores(){
    // Tlo spojne z menu glownym.
    sf::Texture& menuTex = ResourceManager::getTexture(Config::Assets::MAIN_MENU_BG);
    sf::Sprite menuSprite(menuTex);
    menuSprite.setScale(static_cast<float>(Config::WINDOW_WIDTH) / menuTex.getSize().x,
                        static_cast<float>(Config::WINDOW_HEIGHT) / menuTex.getSize().y);
    window.draw(menuSprite);

    if (font.getInfo().family.empty()) return;

    // Naglowek
    sf::Text title("WYNIKI", font, 60);
    title.setFillColor(sf::Color(150, 255, 100));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(3.f);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    title.setPosition(Config::WINDOW_WIDTH / 2.f, 150.f);
    window.draw(title);

    // Tablica wynikow zostanie uzupelniona w MS4 — na razie placeholder.
    sf::Text placeholder("Tablica wynikow - wkrotce (MS4)", font, 26);
    placeholder.setFillColor(sf::Color(200, 200, 200));
    sf::FloatRect phBounds = placeholder.getLocalBounds();
    placeholder.setOrigin(phBounds.width / 2.f, phBounds.height / 2.f);
    placeholder.setPosition(Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT / 2.f);
    window.draw(placeholder);

    // Przycisk powrotu (klasa Button) — działa też ESC/M.
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    hsBackButton.draw(window, hsBackButton.contains(m));
}


void Game::renderUI() {
    // Zabezpieczenie: jeśli czcionka się nie wczytała, nie rysujemy napisów
    if (font.getInfo().family.empty()) return;

    // ==========================================
    // 1. GÓRNY PASEK STATYSTYK (TOP BAR)
    // ==========================================

    // Tło paska
    sf::RectangleShape topBar(sf::Vector2f(Config::WINDOW_WIDTH, 50.f));
    topBar.setFillColor(sf::Color(20, 20, 20, 230)); // Ciemnoszary, lekko przezroczysty
    topBar.setOutlineThickness(2.f);
    topBar.setOutlineColor(sf::Color(80, 80, 80));
    window.draw(topBar);

    // Życia (Po lewej)
    sf::Text livesText("ZYCIA: " + std::to_string(player.getLives()), font, 24);
    livesText.setPosition(20.f, 10.f);
    livesText.setFillColor(sf::Color(255, 80, 80)); // Czerwony
    livesText.setStyle(sf::Text::Bold);

    // Kasa (Bliżej lewej)
    sf::Text moneyText("KASA: $" + std::to_string(player.getMoney()), font, 24);
    moneyText.setPosition(250.f, 10.f);
    moneyText.setFillColor(sf::Color(255, 215, 0)); // Złoty
    moneyText.setStyle(sf::Text::Bold);

    // Punkty (Na środku)
    sf::Text scoreText("PUNKTY: " + std::to_string(player.getScore()), font, 24);
    scoreText.setPosition(500.f, 10.f);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);

    // Fala (Po prawej)
    std::string waveStr = waveManager.isWaveInProgress() ?
                              "FALA: " + std::to_string(waveManager.getCurrentWaveNumber()) + " (W TOKU...)" :
                              "FALA: " + std::to_string(waveManager.getCurrentWaveNumber()) + " [Wcisnij SPACJE]";

    sf::Text waveText(waveStr, font, 24);
    // Pozycjonujemy do prawej krawędzi (zakładając szerokość okna 1280)
    waveText.setPosition(880.f, 10.f);
    waveText.setFillColor(sf::Color::Cyan);
    waveText.setStyle(sf::Text::Bold);

    // Rysujemy statystyki
    window.draw(moneyText);
    window.draw(livesText);
    window.draw(scoreText);
    window.draw(waveText);

    // 4. Wyświetlanie menu budowania TYLKO gdy zaznaczono slot
    if (selectedSlotIndex >= 0) {
        const TowerSlot& slot = map.getSlots()[selectedSlotIndex];

        // Tło pod menu
        sf::RectangleShape menuBg(sf::Vector2f(280.f, 250.f));
        menuBg.setPosition(10.f, 70.f);
        menuBg.setFillColor(sf::Color(30, 30, 50, 210)); // Ciemnogranatowe, przezroczyste tło
        menuBg.setOutlineThickness(2.f);
        menuBg.setOutlineColor(sf::Color(100, 150, 255));
        window.draw(menuBg);

        // Pozycja kursora do podświetlania przycisków HUD.
        sf::Vector2f hudMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (!slot.occupied) {
            // MENU BUDOWANIA — przyciski (klasa Button); klawisze 1-5 też działają.
            sf::Text buildTitle("WYBIERZ WIEZE:", font, 18);
            buildTitle.setPosition(25.f, 80.f);
            buildTitle.setFillColor(sf::Color::White);
            window.draw(buildTitle);

            for (int i = 0; i < 5; ++i)
                buildButtons[i].draw(window, buildButtons[i].contains(hudMouse));

            sf::Text cancelHint("[ESC / PPM] Anuluj", font, 15);
            cancelHint.setPosition(40.f, 290.f);
            cancelHint.setFillColor(sf::Color(180, 180, 180));
            window.draw(cancelHint);
        } else {
            // MENU ULEPSZEŃ / SPRZEDAŻY
            // Musimy znaleźć wieżę, żeby pobrać jej statystyki
            Tower* selectedTower = nullptr;
            for (auto& obj : objects) {
                Tower* t = dynamic_cast<Tower*>(obj.get());
                if (t && t->isAlive() && t->getPosition() == slot.position) {
                    selectedTower = t;
                    break;
                }
            }

            if (selectedTower) {
                int upgCost = selectedTower->getUpgradeCost();
                int refund = static_cast<int>(selectedTower->getCost() * Config::SELL_REFUND);
                int lvl = selectedTower->getLevel();

                // Nagłówek panelu z poziomem wieży.
                sf::Text upgTitle("WIEZA - POZIOM " + std::to_string(lvl), font, 18);
                upgTitle.setPosition(25.f, 80.f);
                upgTitle.setFillColor(sf::Color(255, 150, 255));
                window.draw(upgTitle);

                // Dynamiczne teksty przycisków (koszt zależy od wieży i poziomu).
                upgradeButton.setText((lvl < 3) ? "U: Ulepsz ($" + std::to_string(upgCost) + ")"
                                                : "U: MAX POZIOM");
                sellButton.setText("S: Sprzedaj ($" + std::to_string(refund) + ")");

                upgradeButton.draw(window, upgradeButton.contains(hudMouse));
                sellButton.draw(window, sellButton.contains(hudMouse));

                sf::Text cancelHint("[ESC / PPM] Anuluj", font, 15);
                cancelHint.setPosition(40.f, 215.f);
                cancelHint.setFillColor(sf::Color(180, 180, 180));
                window.draw(cancelHint);
            }
        }
    }
    // --- NAPISY NA MAPIE (BAZA i DROP ZONE) ---
    if (!font.getInfo().family.empty()) {
        // 1. Napis DROP ZONE-> teraz H
        sf::Text dzText("H", font, 87);
        dzText.setFillColor(sf::Color(0, 255, 0, 150));
        // Pobieramy pozycję strefy z naszej aktualnej mapy
        sf::Vector2f dzPos = map.getDropZonePos();
        // Ustawiamy pozycję tekstu delikatnie przesuniętą
        dzText.setPosition(dzPos.x - 25.f, dzPos.y - 45.f);
        window.draw(dzText);

        /*// 2. Napis BAZA (Pionowo)
        sf::Text baseText("G\nA\nT\nE", font, 22);
        baseText.setFillColor(sf::Color::White);
        baseText.setStyle(sf::Text::Bold);

        sf::Vector2f basePos = map.getWaypoints().back(); // Ostatni waypoint to baza
        sf::FloatRect baseBounds = baseText.getLocalBounds();
        baseText.setOrigin(baseBounds.width / 0.5, baseBounds.height / 2.f);
        baseText.setPosition(basePos.x, basePos.y);
        window.draw(baseText);*/
    }
}

void Game::renderPlaying() {
    // UWAGA: brak window.clear()/window.display() — robi to render(),
    // który jest dyrygentem. Podwójny display() na klatkę zamieniał bufory
    // dwa razy i powodował miganie (stary bufor menu vs. świeża gra).
    map.draw(window);


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
        renderUI();
}

void Game::render() {
    window.clear(sf::Color::Black);

    switch (state) {
    case GameState::MENU:
        renderMenu();
        break;

    case GameState::MAP_SELECT:
        renderMapSelect();
        break;

    case GameState::PLAYING:
        renderPlaying();
        break;

    case GameState::PAUSED:
        renderPlaying();  // tlo - zatrzymana rozgrywka
        renderPaused();   // nakladka
        break;

    case GameState::GAME_OVER:
        renderPlaying();   // tlo
        renderGameOver();  // nakladka
        break;

    case GameState::HIGHSCORES:
        renderHighscores();
        break;
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

void Game::tryUpgradeTower() {
    if (selectedSlotIndex < 0) return;
    TowerSlot& slot = map.getSlots()[selectedSlotIndex];
    if (!slot.occupied) return; // Ulepszyć można tylko zajęty slot

    // Szukamy wieży, która stoi na tym konkretnym slocie
    for (auto& obj : objects) {
        Tower* tower = dynamic_cast<Tower*>(obj.get());
        if (tower && tower->isAlive() && tower->getPosition() == slot.position) {

            if (tower->getLevel() >= 3) return; // Osiągnięto max poziom

            int upgradeCost = tower->getUpgradeCost();
            if (player.spendMoney(upgradeCost)) {
                tower->upgrade();
            }
            break;
        }
    }
}

void Game::trySellTower() {
    if (selectedSlotIndex < 0) return;
    TowerSlot& slot = map.getSlots()[selectedSlotIndex];
    if (!slot.occupied) return; // Sprzedać można tylko zajęty slot

    // Szukamy wieży na slocie
    for (auto& obj : objects) {
        Tower* tower = dynamic_cast<Tower*>(obj.get());
        if (tower && tower->isAlive() && tower->getPosition() == slot.position) {

            // Zwracamy graczowi 70% całkowitej wartości wieży (bazowa + ulepszenia)
            int refund = static_cast<int>(tower->getCost() * Config::SELL_REFUND);
            player.addMoney(refund);

            // Niszczymy wieżę i zwalniamy slot
            tower->destroy();
            slot.occupied = false;
            selectedSlotIndex = -1; // Odznaczamy slot
            break;
        }
    }
}

// ZAPIS I ODCZYT GRY
void Game::saveGame() {
    std::ofstream file("save.txt");
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku do zapisu!" << std::endl;
        return;
    }

    // 0. Zapisujemy ścieżkę mapy (osobna linia — może zawierać spacje).
    file << currentMapPath << "\n";

    // 1. Zapisujemy Gracza i aktualną falę z WaveManager
    file << player.getMoney() << " " << player.getLives() << " "
         << player.getScore() << " " << waveManager.getCurrentWaveIndex() << "\n";

    // 2. Szukamy wszystkich wież na mapie
    std::vector<Tower*> towersToSave;
    for (auto& obj : objects) {
        if (Tower* t = dynamic_cast<Tower*>(obj.get())) {
            towersToSave.push_back(t);
        }
    }

    // 3. Zapisujemy liczbę wież, a potem ich dane (Typ, X, Y, Poziom)
    file << towersToSave.size() << "\n";
    for (Tower* t : towersToSave) {
        int type = 0;
        if (dynamic_cast<MachineGunTower*>(t)) type = 1;
        else if (dynamic_cast<SniperTower*>(t)) type = 2;
        else if (dynamic_cast<RocketTower*>(t)) type = 3;
        else if (dynamic_cast<FlamethrowerTower*>(t)) type = 4;
        else if (dynamic_cast<SlowerTower*>(t)) type = 5;

        file << type << " " << t->getPosition().x << " " << t->getPosition().y << " " << t->getLevel() << "\n";
    }

    file.close();
    std::cout << "Zapisano gre pomyslnie! (Klawisz F5)" << std::endl;
}

void Game::loadGame() {
    std::ifstream file("save.txt");
    if (!file.is_open()) {
        std::cerr << "Brak pliku zapisu!" << std::endl;
        return;
    }

    // 1. Czyścimy obecny stan gry
    objects.clear();

    // Wczytujemy ścieżkę mapy (pierwsza linia zapisu) i ładujemy tę planszę.
    std::getline(file, currentMapPath);
    map.loadFromFile(std::string(ASSETS_DIR) + currentMapPath); // Reset slotów do stanu "puste"
    selectedSlotIndex = -1;
    state = GameState::PLAYING;

    // 2. Wczytujemy gracza i numer fali
    int money, lives, score, waveIndex, towerCount;
    file >> money >> lives >> score >> waveIndex;

    player.setMoney(money);
    player.setLives(lives);
    player.setScore(score);
    waveManager.setCurrentWaveIndex(waveIndex);

    // 3. Wczytujemy wieże
    file >> towerCount;
    for (int i = 0; i < towerCount; ++i) {
        int type, level;
        float px, py;
        file >> type >> px >> py >> level;

        sf::Vector2f pos(px, py);
        Tower* newTower = nullptr;

        // Tworzymy odpowiedni obiekt
        if (type == 1) { auto t = std::make_unique<MachineGunTower>(pos); newTower = t.get(); objects.push_back(std::move(t)); }
        else if (type == 2) { auto t = std::make_unique<SniperTower>(pos); newTower = t.get(); objects.push_back(std::move(t)); }
        else if (type == 3) { auto t = std::make_unique<RocketTower>(pos); newTower = t.get(); objects.push_back(std::move(t)); }
        else if (type == 4) { auto t = std::make_unique<FlamethrowerTower>(pos); newTower = t.get(); objects.push_back(std::move(t)); }
        else if (type == 5) { auto t = std::make_unique<SlowerTower>(pos); newTower = t.get(); objects.push_back(std::move(t)); }

        // Zajmujemy odpowiedni slot na mapie
        for (auto& slot : map.getSlots()) {
            if (slot.position == pos) {
                slot.occupied = true;
                break;
            }
        }

        // Ulepszamy wczytaną wieżę do odpowiedniego poziomu
        if (newTower) {
            for (int lvl = 1; lvl < level; ++lvl) {
                newTower->upgrade();
            }
        }
    }

    file.close();
    std::cout << "Wczytano gre pomyslnie! (Klawisz F9)" << std::endl;
}
