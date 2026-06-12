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
#include <algorithm>

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
    initButtons();   // przyciski ekranów wymagają wczytanej czcionki
    hud.init(font);  // interfejs gracza (pasek + panel wież)
}

void Game::initButtons() {
    const float cx = Config::WINDOW_WIDTH / 2.f;  // środek poziomy okna
    const sf::Vector2f bigSize(360.f, 50.f);      // standardowy rozmiar przycisku menu

    // --- MENU GŁÓWNE ---
    const char* menuLabels[5] = { "Nowa gra", "Wczytaj gre", "Wyniki", "Wyjdz", "Zasady Gry" };
    float y = 360.f;
    for (int i = 0; i < 5; ++i) {
        menuButtons[i].setup(font, menuLabels[i], {cx, y}, bigSize, 30);
        y += 65.f;
    }
    // Używamy tego samego przycisku powrotu (hsBackButton) dla ekranu Help!
    hsBackButton.setup(font, "Powrot", {cx, y}, bigSize, 30);

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
    gameOverButtons[0].setup(font, "Zagraj ponownie (R)", {cx, Config::WINDOW_HEIGHT / 2.f + 150.f}, bigSize, 28);
    gameOverButtons[1].setup(font, "Menu glowne (M)", {cx, Config::WINDOW_HEIGHT / 2.f + 215.f}, bigSize, 28);

    // --- EKRAN WYNIKÓW ---
    hsBackButton.setup(font, "Powrot do menu (ESC)", {cx, Config::WINDOW_HEIGHT - 60.f}, {320.f, 46.f}, 22);

    // Przyciski HUD (panel budowy/ulepszeń) konfiguruje sama klasa HUD w init().
}

void Game::startNewGame() {
    objects.clear();           // usuń wszystkie zombi, wieże, pociski
    player.reset();            // reset gracza do startowych wartości
    map.loadFromFile(std::string(ASSETS_DIR) + currentMapPath);  // przeładuj wybraną mapę (zwalnia sloty)
    spawnTimer = 0.f;
    selectedSlotIndex = -1;    // anuluj wybór slotu jeśli był
    state = GameState::PLAYING;
    nicknameInput.clear();     // wyczyść pole nicku z poprzedniej przegranej
    scoreSaved = false;
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
                for (int i = 0; i < 5; ++i)
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
                    for (int i = 0; i < 5; ++i)
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
                    // Opuszczając ekran zapisujemy wynik, jeśli gracz tego nie zrobił Enterem.
                    if (gameOverButtons[0].contains(mouse)) { if (!scoreSaved) saveScore(); startNewGame(); }
                    else if (gameOverButtons[1].contains(mouse)) { if (!scoreSaved) saveScore(); state = GameState::MENU; menuSelectedOption = 0; }
                }
                break;

            case GameState::HIGHSCORES:
                if (left && hsBackButton.contains(mouse)) { state = GameState::MENU; menuSelectedOption = 0; }
                break;

            case GameState::HELP:
                if (left && hsBackButton.contains(mouse)) { state = previousState; }
                break;

            case GameState::PLAYING:
                if (left) {
                    // Sprawdzamy interfejs HUD (niezależnie od tego, czy slot jest zaznaczony)
                    bool isOccupied = (selectedSlotIndex >= 0) ? map.getSlots()[selectedSlotIndex].occupied : false;
                    HudClick hc = hud.handleClick(mouse, isOccupied);

                    if (hc.action == HudAction::HELP) {
                        previousState = state;     // Zapisujemy, że pomoc włączono w trakcie gry
                        state = GameState::HELP;   // Gra się pauzuje i wyświetla instrukcję
                        break;                     // Przerywamy sprawdzanie innych kliknięć
                    }
                    else if (hc.action != HudAction::NONE && selectedSlotIndex >= 0) {
                        switch (hc.action) {
                        case HudAction::BUY_TOWER: tryBuyTower(hc.towerType); break;
                        case HudAction::UPGRADE:   tryUpgradeTower(); break;
                        case HudAction::SELL:      trySellTower(); break;
                        default: break;
                        }
                        break;  // Kliknięto w menu budowy, nie zaznaczamy slotu na mapie

                    }

                    // Kliknięcie w bonus (paczkę) na mapie.
                    for (auto& obj : objects) {
                        Bonus* bonus = dynamic_cast<Bonus*>(obj.get());
                        if (bonus && bonus->isAlive() && bonus->contains(mouse)) {
                            applyBonus(bonus->getType(), bonus->getPosition());
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

        // 2c. WPISYWANIE NICKU NA EKRANIE GAME OVER
        case sf::Event::TextEntered:
            // Tylko gdy gracz przegrał i jeszcze nie zapisał wyniku.
            if (state == GameState::GAME_OVER && !scoreSaved) {
                sf::Uint32 c = event.text.unicode;
                if (c == 8) {                         // Backspace — usuń ostatni znak
                    if (!nicknameInput.empty()) nicknameInput.pop_back();
                } else if (c >= 32 && c < 127 && c != ';') {
                    // Akceptujemy drukowalne znaki ASCII; ';' jest separatorem w pliku.
                    if (nicknameInput.size() < 12)    // limit długości nicku
                        nicknameInput += static_cast<char>(c);
                }
            }
            break;

        // 3. OBSŁUGA KLAWIATURY
        case sf::Event::KeyPressed:
            // Dzielimy logikę klawiszy w zależności od obecnego stanu gry
            switch (state) {
            case GameState::MENU:
                if (event.key.code == sf::Keyboard::Up) {
                    menuSelectedOption = (menuSelectedOption > 0) ? menuSelectedOption - 1 : 4;
                } else if (event.key.code == sf::Keyboard::Down) {
                    menuSelectedOption = (menuSelectedOption < 4) ? menuSelectedOption + 1 : 0;
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
                if (!scoreSaved) {
                    // Faza wpisywania nicku — Enter zatwierdza i zapisuje wynik.
                    // R/M są zablokowane, by litery z nicku nie restartowały gry.
                    if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
                        saveScore();
                    }
                } else if (event.key.code == sf::Keyboard::R) {
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

            case GameState::HELP:
                if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (hsBackButton.contains(mousePos)) {
                    state = previousState; // <--- ZMIANA: Wracamy dokładnie tam, skąd przyszliśmy!
                }
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
        highscores = loadHighscores();  // odśwież listę z pliku przy każdym wejściu
        state = GameState::HIGHSCORES;
        break;

    case 3:  // Wyjdz
        window.close();
        break;

    case 4:  // Pomoc
        previousState = state;     // Zapisujemy, gdzie byliśmy
        state = GameState::HELP;
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

    // Obsługa błękitnego błysku
    if (freezeTintTimer > 0.f) {
        freezeTintTimer -= dt;
    }

    // Obsługa lecących ikonek
    for (auto it = flyingIcons.begin(); it != flyingIcons.end(); ) {
        it->progress += 2.0f * dt; // Prędkość lotu (2.0 = pół sekundy lotu)
        if (it->progress >= 1.0f) {
            it = flyingIcons.erase(it); // Usuń po dotarciu
        } else {
            // Animacja płynnego przemieszczania
            sf::Vector2f diff = it->targetPos - it->startPos;
            it->sprite.setPosition(it->startPos + diff * it->progress);
            ++it;
        }
    }


    if (!player.isAlive()) {
        state = GameState::GAME_OVER;
        nicknameInput.clear();   // przygotuj puste pole na nick gracza
        scoreSaved = false;      // wynik tej przegranej jeszcze nie zapisany
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
    for (int i = 0; i < 5; ++i)
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

    // --- POLE NICKU / POTWIERDZENIE ZAPISU ---
    // Przed zapisem prosimy o nick (z migającym kursorem), po zapisie pokazujemy potwierdzenie.
    sf::Text infoText("", font, 24);
    infoText.setFillColor(sf::Color(220, 220, 220));
    if (!scoreSaved) {
        // Migający kursor "_" — własny, nieresetowany zegar (clock jest restartowany co klatkę).
        static sf::Clock caretClock;
        bool caret = static_cast<int>(caretClock.getElapsedTime().asSeconds() * 2.f) % 2 == 0;
        infoText.setString("Wpisz nick: " + nicknameInput + (caret ? "_" : " ") +
                           "\n(Enter aby zapisac wynik)");
    } else {
        infoText.setString("Wynik zapisany!\nR - zagraj ponownie   M - menu");
    }
    infoText.setPosition(0.f, 0.f);
    sf::FloatRect ib = infoText.getLocalBounds();
    infoText.setOrigin(ib.left + ib.width / 2.f, ib.top + ib.height / 2.f);
    infoText.setPosition(Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT / 2.f + 75.f);
    window.draw(infoText);

    // Przyciski (klasa Button) — klawisze R/M działają po zapisaniu wyniku, mysz podświetla.
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

    if (highscores.empty()) {
        // Brak zapisanych wyników — komunikat zamiast tabeli.
        sf::Text empty("Brak wynikow - rozegraj partie!", font, 26);
        empty.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect eb = empty.getLocalBounds();
        empty.setOrigin(eb.width / 2.f, eb.height / 2.f);
        empty.setPosition(Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT / 2.f);
        window.draw(empty);
    } else {
        // Tabela top 10: pozycja + nick po lewej, mapa na środku, punkty po prawej.
        const float startY = 240.f;     // Y pierwszego wiersza
        const float rowH   = 38.f;      // odstęp między wierszami
        const float colNick = Config::WINDOW_WIDTH / 2.f - 320.f;  // lewa krawędź kolumny nick
        const float colMap  = Config::WINDOW_WIDTH / 2.f + 40.f;   // kolumna nazwy mapy
        const float colScore = Config::WINDOW_WIDTH / 2.f + 320.f; // prawa krawędź punktów

        for (std::size_t i = 0; i < highscores.size(); ++i) {
            const ScoreEntry& e = highscores[i];
            float y = startY + i * rowH;
            // Podium (1-3 miejsce) wyróżnione kolorem, reszta biała.
            sf::Color rowColor = sf::Color::White;
            if (i == 0)      rowColor = sf::Color(255, 215, 0);    // złoto
            else if (i == 1) rowColor = sf::Color(200, 200, 200);  // srebro
            else if (i == 2) rowColor = sf::Color(205, 127, 50);   // brąz

            // Pozycja + nick (wyrównane do lewej).
            sf::Text left(std::to_string(i + 1) + ". " + e.nick, font, 24);
            left.setFillColor(rowColor);
            left.setPosition(colNick, y);
            window.draw(left);

            // Nazwa mapy (wyrównana do lewej, środkowa kolumna).
            sf::Text mapText(e.mapName, font, 22);
            mapText.setFillColor(rowColor);
            mapText.setPosition(colMap, y + 1.f);
            window.draw(mapText);

            // Punkty (wyrównane do prawej — origin na prawej krawędzi tekstu).
            sf::Text scoreText(std::to_string(e.score), font, 24);
            scoreText.setFillColor(rowColor);
            sf::FloatRect sb = scoreText.getLocalBounds();
            scoreText.setOrigin(sb.left + sb.width, 0.f);
            scoreText.setPosition(colScore, y);
            window.draw(scoreText);
        }
    }

    // Przycisk powrotu (klasa Button) — działa też ESC/M.
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    hsBackButton.draw(window, hsBackButton.contains(m));
}

void Game::renderHelp() {
    // 1.  możemy narysować tło z menu, żeby było ładniej
    sf::Texture& menuTex = ResourceManager::getTexture(Config::Assets::MAIN_MENU_BG);
    sf::Sprite menuSprite(menuTex);
    float scaleX = static_cast<float>(Config::WINDOW_WIDTH) / menuTex.getSize().x;
    float scaleY = static_cast<float>(Config::WINDOW_HEIGHT) / menuTex.getSize().y;
    menuSprite.setScale(scaleX, scaleY);
    window.draw(menuSprite);

    // 2. Ciemne, półprzezroczyste tło dla czytelności tekstu
    sf::RectangleShape bgPanel(sf::Vector2f(1000.f, 550.f));
    bgPanel.setOrigin(500.f, 275.f);
    bgPanel.setPosition(Config::WINDOW_WIDTH / 2.f, Config::WINDOW_HEIGHT / 2.f - 10.f);
    bgPanel.setFillColor(sf::Color(20, 20, 20, 220));
    bgPanel.setOutlineThickness(3.f);
    bgPanel.setOutlineColor(sf::Color(100, 150, 255));
    window.draw(bgPanel);

    // 3. Tytuł
    sf::Text title("ZASADY GRY", font, 40);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    title.setOrigin(title.getLocalBounds().width / 2.f, 0.f);
    title.setPosition(Config::WINDOW_WIDTH / 2.f, 90.f);
    window.draw(title);

    // 4. Treść instrukcji
    std::string rulesText =
        "CEL GRY:\n"
        "Nie pozwol, aby zombie dotarly do Twojej bazy (brama na koncu drogi).\n"
        "Masz 20 zyc. Kazdy zombie zabiera 1 lub wiecej zyc.\n\n"

        "STEROWANIE:\n"
        "- Kliknij LEWYM przyciskiem na szary kwadrat (slot), aby wybrac miejsce na wieze.\n"
        "- Uzyj menu po lewej stronie, aby kupic wieze za dolary ($).\n"
        "- Kliknij na zbudowana wieze, aby ja ULEPSZYC lub SPRZEDAC.\n"
        "- ESC / PRAWY przycisk myszy - odznacza wybrany slot.\n"
        "- ESC (gdy slot nie jest zaznaczony) - PAUZUJE GRE.\n"
        "- SPACJA - wypuszcza kolejna fale zombie.\n\n"

        "ZOMBIE:\n"
        "- Walker: Zwykly zombie.\n"
        "- Runner: Bardzo szybki, ale malo odporny.\n"
        "- Armored: Powolny, posiada pancerz dajacy 50% odpornosci na kule!\n"
        "- Tank: Ogromny boss z gigantyczna iloscia HP.\n\n"

        "WSKAZOWKA: Obserwuj zrzuty zaopatrzenia w strefie \"H\"!";

    sf::Text content(rulesText, font, 16);
    content.setFillColor(sf::Color::White);
    content.setPosition(200.f, 160.f);
    content.setLineSpacing(1.2f); // Zwiększa odstępy między linijkami dla czytelności
    window.draw(content);

    // 5. Przycisk powrotu
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    hsBackButton.draw(window, hsBackButton.contains(mousePos));
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

    // Interfejs gracza rysuje osobna klasa HUD.
    hud.draw(window, player, waveManager, map, objects, selectedSlotIndex);

    // Błękitny błysk ekranu (EMP / LÓD) - Rysowany NAD mapą i pod HUDem (lub na wszystkim)
    if (freezeTintTimer > 0.f) {
        sf::RectangleShape tint(sf::Vector2f(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));
        int alpha = static_cast<int>(freezeTintTimer * 100.f);
        if (alpha > 100) alpha = 100;
        if (alpha < 0) alpha = 0;
        tint.setFillColor(sf::Color(100, 200, 255, alpha)); // Przezroczysty lodowy
        window.draw(tint);
    }

    // Lecące ikonki rysowane ZUPEŁNIE na wierzchu!
    for (auto& fi : flyingIcons) {
        window.draw(fi.sprite);
    }
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

    case GameState::HELP:
        renderHelp();
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

void Game::applyBonus(BonusType type, sf::Vector2f startPos) {
    if (type == BonusType::AMMO) {
        player.addMoney(Config::AMMO_BONUS_VALUE); // +30 dolarów
        // Tworzy monetę, która leci do ikonki "Kasa"
        FlyingIcon fi;
        fi.sprite.setTexture(ResourceManager::getTexture(Config::Assets::ICON_COIN), true);
        float s = 30.f / fi.sprite.getTexture()->getSize().x;
        fi.sprite.setScale(s, s);
        fi.sprite.setOrigin(fi.sprite.getTexture()->getSize().x / 2.f, fi.sprite.getTexture()->getSize().y / 2.f);
        fi.startPos = startPos;
        fi.targetPos = sf::Vector2f(290.f, 25.f); // Pozycja ikonki monety na pasku
        flyingIcons.push_back(fi);
    }
    else if (type == BonusType::MEDKIT) {
        player.addLives(Config::MEDKIT_BONUS_VALUE); // +1 życie
        // Tworzy serce, które leci do ikonki "Życia"
        FlyingIcon fi;
        fi.sprite.setTexture(ResourceManager::getTexture(Config::Assets::ICON_HEART), true);
        float s = 30.f / fi.sprite.getTexture()->getSize().x;
        fi.sprite.setScale(s, s);
        fi.sprite.setOrigin(fi.sprite.getTexture()->getSize().x / 2.f, fi.sprite.getTexture()->getSize().y / 2.f);
        fi.startPos = startPos;
        fi.targetPos = sf::Vector2f(35.f, 25.f); // Pozycja ikonki na pasku
        flyingIcons.push_back(fi);

    } else if (type == BonusType::EMP) {
        // Efekt EMP: błękitny ekran na 1.5 sekundy
        freezeTintTimer = 1.5f;
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

// Zwraca czytelną nazwę mapy z Config::Maps pasującą do bieżącej ścieżki.
std::string Game::currentMapName() const {
    for (int i = 0; i < Config::Maps::COUNT; ++i)
        if (currentMapPath == Config::Maps::PATHS[i])
            return Config::Maps::NAMES[i];
    return "Nieznana";
}

// Dopisuje jeden wiersz wyniku do highscores.txt w formacie: nick;mapa;punkty
// Plik otwierany w trybie dopisywania (app), więc kolejne wyniki się kumulują.
// Odczyt i ranking top 10 dodamy w kolejnym kroku.
void Game::saveScore() {
    if (scoreSaved) return;  // zabezpieczenie przed podwójnym zapisem tej samej przegranej

    // Pusty nick zastępujemy domyślnym, by w pliku nie powstał wiersz bez nazwy.
    std::string nick = nicknameInput.empty() ? "Gracz" : nicknameInput;

    std::ofstream file("highscores.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc highscores.txt do zapisu!" << std::endl;
        return;
    }

    file << nick << ";" << currentMapName() << ";" << player.getScore() << "\n";
    file.close();

    scoreSaved = true;
    std::cout << "Zapisano wynik: " << nick << " (" << currentMapName()
              << ") - " << player.getScore() << " pkt" << std::endl;
}

std::vector<ScoreEntry> Game::loadHighscores() const {
    std::vector<ScoreEntry> entries;
    std::ifstream file("highscores.txt");
    if (!file.is_open()) return entries;  // brak pliku = pusta tablica wyników

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        // Format wiersza: nick;mapa;punkty (nick i mapa nie zawierają ';').
        std::size_t p1 = line.find(';');
        std::size_t p2 = line.rfind(';');
        if (p1 == std::string::npos || p2 == p1) continue;  // pomijamy uszkodzone wiersze

        ScoreEntry e;
        e.nick    = line.substr(0, p1);
        e.mapName = line.substr(p1 + 1, p2 - p1 - 1);
        try {
            e.score = std::stoi(line.substr(p2 + 1));
        } catch (...) {
            continue;  // pomijamy wiersz z niepoprawną liczbą punktów
        }
        entries.push_back(e);
    }
    file.close();

    // Sortujemy malejąco po wyniku i zostawiamy maksymalnie 10 najlepszych.
    std::sort(entries.begin(), entries.end(),
              [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    if (entries.size() > 10) entries.resize(10);
    return entries;
}
