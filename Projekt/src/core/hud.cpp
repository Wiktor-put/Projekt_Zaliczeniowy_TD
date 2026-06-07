#include "hud.h"
#include "Config.h"
#include "player.h"
#include "wavemanager.h"
#include "map.h"
#include "Gameobject.h"
#include "tower.h"
#include <string>

void HUD::init(const sf::Font& f) {
    font = &f;

    // Panel budowy/ulepszeń w lewym górnym rogu — środek panelu na x=150.
    const float panelCx = 150.f;
    const sf::Vector2f hudSize(260.f, 28.f);
    // Koszty pobierane z Config — brak "magic numbers", zmiana balansu w jednym miejscu.
    const std::string buildLabels[5] = {
        "1: Karabin ($"      + std::to_string(Config::MachineGunTower::COST)   + ")",
        "2: Snajper ($"      + std::to_string(Config::SniperTower::COST)       + ")",
        "3: Wyrzutnia ($"    + std::to_string(Config::RocketTower::COST)       + ")",
        "4: Miotacz ognia ($" + std::to_string(Config::FlamethrowerTower::COST) + ")",
        "5: Spowalniacz ($"  + std::to_string(Config::SlowerTower::COST)       + ")"
    };
    float hy = 120.f;
    for (int i = 0; i < 5; ++i) {
        buildButtons[i].setup(f, buildLabels[i], {panelCx, hy}, hudSize, 16);
        hy += 34.f;
    }
    // Teksty ulepszenia/sprzedaży są dynamiczne — ustawiamy je co klatkę w draw().
    upgradeButton.setup(f, "", {panelCx, 130.f}, {260.f, 34.f}, 16);
    sellButton.setup(f, "", {panelCx, 175.f}, {260.f, 34.f}, 16);
    // Przycisk pomocy w lewym rogu (znak zapytania)
    helpButton.setup(f, "?", {40.f, 680.f}, {40.f, 40.f}, 24);
}

void HUD::draw(sf::RenderWindow& window,
               const Player& player, const WaveManager& wave, const Map& map,
               const std::vector<std::unique_ptr<GameObject>>& objects,
               int selectedSlotIndex) const {
    // Bez czcionki nie rysujemy napisów ani przycisków.
    if (!font || font->getInfo().family.empty()) return;

    // ==========================================
    // 1. GÓRNY PASEK STATYSTYK (TOP BAR)
    // ==========================================
    sf::RectangleShape topBar(sf::Vector2f(static_cast<float>(Config::WINDOW_WIDTH), 50.f));
    topBar.setFillColor(sf::Color(20, 20, 20, 230));
    topBar.setOutlineThickness(2.f);
    topBar.setOutlineColor(sf::Color(80, 80, 80));
    window.draw(topBar);

    sf::Text livesText("ZYCIA: " + std::to_string(player.getLives()), *font, 24);
    livesText.setPosition(20.f, 10.f);
    livesText.setFillColor(sf::Color(255, 80, 80));
    livesText.setStyle(sf::Text::Bold);

    sf::Text moneyText("KASA: $" + std::to_string(player.getMoney()), *font, 24);
    moneyText.setPosition(250.f, 10.f);
    moneyText.setFillColor(sf::Color(255, 215, 0));
    moneyText.setStyle(sf::Text::Bold);

    sf::Text scoreText("PUNKTY: " + std::to_string(player.getScore()), *font, 24);
    scoreText.setPosition(500.f, 10.f);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);

    std::string waveStr = wave.isWaveInProgress() ?
        "FALA: " + std::to_string(wave.getCurrentWaveNumber()) + " (W TOKU...)" :
        "FALA: " + std::to_string(wave.getCurrentWaveNumber()) + " [Wcisnij SPACJE]";
    sf::Text waveText(waveStr, *font, 24);
    waveText.setPosition(880.f, 10.f);
    waveText.setFillColor(sf::Color::Cyan);
    waveText.setStyle(sf::Text::Bold);

    window.draw(moneyText);
    window.draw(livesText);
    window.draw(scoreText);
    window.draw(waveText);

    // Rysowanie przycisku pomocy (sprawdzamy czy kursor na nim jest)
    sf::Vector2f hudMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    helpButton.draw(window, helpButton.contains(hudMouse));

    // ==========================================
    // 2. PANEL SLOTU (tylko gdy zaznaczono slot)
    // ==========================================
    if (selectedSlotIndex >= 0) {
        const TowerSlot& slot = map.getSlots()[selectedSlotIndex];

        sf::RectangleShape menuBg(sf::Vector2f(280.f, 250.f));
        menuBg.setPosition(10.f, 70.f);
        menuBg.setFillColor(sf::Color(30, 30, 50, 210));
        menuBg.setOutlineThickness(2.f);
        menuBg.setOutlineColor(sf::Color(100, 150, 255));
        window.draw(menuBg);

        // Kursor do podświetlania przycisków.
        sf::Vector2f hudMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (!slot.occupied) {
            // MENU BUDOWANIA — przyciski (klawisze 1-5 obsługiwane przez Game).
            sf::Text buildTitle("WYBIERZ WIEZE:", *font, 18);
            buildTitle.setPosition(25.f, 80.f);
            buildTitle.setFillColor(sf::Color::White);
            window.draw(buildTitle);

            for (int i = 0; i < 5; ++i)
                buildButtons[i].draw(window, buildButtons[i].contains(hudMouse));

            sf::Text cancelHint("[ESC / PPM] Anuluj", *font, 15);
            cancelHint.setPosition(40.f, 290.f);
            cancelHint.setFillColor(sf::Color(180, 180, 180));
            window.draw(cancelHint);
        } else {
            // MENU ULEPSZEŃ / SPRZEDAŻY — znajdujemy wieżę stojącą w tym slocie.
            const Tower* selectedTower = nullptr;
            for (const auto& obj : objects) {
                const Tower* t = dynamic_cast<const Tower*>(obj.get());
                if (t && t->isAlive() && t->getPosition() == slot.position) {
                    selectedTower = t;
                    break;
                }
            }

            if (selectedTower) {
                int upgCost = selectedTower->getUpgradeCost();
                int refund = static_cast<int>(selectedTower->getCost() * Config::SELL_REFUND);
                int lvl = selectedTower->getLevel();

                sf::Text upgTitle("WIEZA - POZIOM " + std::to_string(lvl), *font, 18);
                upgTitle.setPosition(25.f, 80.f);
                upgTitle.setFillColor(sf::Color(255, 150, 255));
                window.draw(upgTitle);

                // Teksty zależne od wieży i poziomu. Mutujemy kopie przycisków,
                // by metoda mogła pozostać const (HUD nie zmienia własnego stanu).
                Button upg = upgradeButton;
                Button sell = sellButton;
                upg.setText((lvl < 3) ? "U: Ulepsz ($" + std::to_string(upgCost) + ")"
                                      : "U: MAX POZIOM");
                sell.setText("S: Sprzedaj ($" + std::to_string(refund) + ")");

                upg.draw(window, upg.contains(hudMouse));
                sell.draw(window, sell.contains(hudMouse));

                sf::Text cancelHint("[ESC / PPM] Anuluj", *font, 15);
                cancelHint.setPosition(40.f, 215.f);
                cancelHint.setFillColor(sf::Color(180, 180, 180));
                window.draw(cancelHint);
            }
        }
    }

    // ==========================================
    // 3. NAPIS STREFY ZRZUTU (DROP ZONE -> "H")
    // ==========================================
    sf::Text dzText("H", *font, 87);
    dzText.setFillColor(sf::Color(0, 255, 0, 150));
    sf::Vector2f dzPos = map.getDropZonePos();
    dzText.setPosition(dzPos.x - 25.f, dzPos.y - 45.f);
    window.draw(dzText);
}

HudClick HUD::handleClick(sf::Vector2f mouse, bool occupied) const {
    HudClick result;
    // Sprawdzamy przycisk pomocy niezależnie od zaznaczonego slotu
    if (helpButton.contains(mouse)) {
        result.action = HudAction::HELP;
        return result;
    }

    if (!occupied) {
        for (int i = 0; i < 5; ++i)
            if (buildButtons[i].contains(mouse)) {
                result.action = HudAction::BUY_TOWER;
                result.towerType = i + 1;
                return result;
            }
    } else {
        if (upgradeButton.contains(mouse)) result.action = HudAction::UPGRADE;
        else if (sellButton.contains(mouse)) result.action = HudAction::SELL;
    }
    return result;
}
