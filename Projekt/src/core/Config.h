#ifndef CONFIG_H
#define CONFIG_H

// Config.h — centralne stałe konfiguracyjne gry (okno, zombie, wieże, pociski, fale)

namespace Config {

// ===== OKNO I PĘTLA GRY =====
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int FPS_LIMIT = 60;

// ===== ASSETS (SCIEZKI DO PLIKOW) =====
namespace Assets {
// Tekstura ekranu początkowego
constexpr const char* MAIN_MENU_BG = "/assets/textures/main_menu.png";

constexpr const char* BACKGROUND = "/assets/textures/background_las.png";

// Tekstura ścieżki
constexpr const char* PATH = "/assets/textures/path.png";
// Tekstura strefy zrzutu (Drop Zone)
constexpr const char* DROPZONE = "/assets/textures/H.jpg";
// Tekstura Bazy / Bramy
constexpr const char* BASE_GATE = "/assets/textures/gate.png";
constexpr const char* BASE_WALL_VERT = "/assets/textures/Wooden-wall_Vertical.png";

// --- Ikonki UI i Bonusów ---
constexpr const char* ICON_HEART = "/assets/textures/bonus/heart.png";
constexpr const char* ICON_COIN  = "/assets/textures/bonus/coin.png";
constexpr const char* ICON_STAR  = "/assets/textures/bonus/Green Star.png";
constexpr const char* ICON_ICE   = "/assets/textures/bonus/ice.png";

// Nowe ścieżki dla Walkera
constexpr const char* WALKER_DOWN = "/assets/textures/walker walk/Zombie_Small_Down_walk-Sheet6.png";
constexpr const char* WALKER_UP = "/assets/textures/walker walk/Zombie_Small_Up_Walk-Sheet6.png";
constexpr const char* WALKER_RIGHT = "/assets/textures/walker walk/Zombie_Small_Side_Walk-Sheet6.png";
constexpr const char* WALKER_LEFT = "/assets/textures/walker walk/Zombie_Small_Side-left_Walk-Sheet6.png";

// Ścieżki dla Runnera (Wersja bez topora)
constexpr const char* RUNNER_DOWN = "/assets/textures/amored walk/Zombie_Axe_No-axe_Down_Walk-Sheet8.png";
constexpr const char* RUNNER_UP = "/assets/textures/amored walk/Zombie_Axe_No-axe_Up_Walk-Sheet8.png";
constexpr const char* RUNNER_RIGHT = "/assets/textures/amored walk/Zombie_Axe_No-axe_Side_Walk-Sheet8.png";
constexpr const char* RUNNER_LEFT = "/assets/textures/amored walk/Zombie_Axe_No-axe_Side-left_Walk-Sheet8.png";

// Ścieżki dla Armored Zombie
constexpr const char* ARMORED_DOWN = "/assets/textures/amored walk/Zombie_Axe_Down_Walk-Sheet8.png";
constexpr const char* ARMORED_UP = "/assets/textures/amored walk/Zombie_Axe_Up_Walk-Sheet8.png";
constexpr const char* ARMORED_RIGHT = "/assets/textures/amored walk/Zombie_Axe_Side_Walk-Sheet8.png";
constexpr const char* ARMORED_LEFT = "/assets/textures/amored walk/Zombie_Axe_Side-left_Walk-Sheet8.png";

// Ścieżki dla Tanka
constexpr const char* TANK_DOWN = "/assets/textures/tank walk/Zombie_Big_Down_Walk-Sheet8.png";
constexpr const char* TANK_UP = "/assets/textures/tank walk/Zombie_Big_Up_Walk-Sheet8.png";
constexpr const char* TANK_RIGHT = "/assets/textures/tank walk/Zombie_Big_Side_Walk-Sheet8.png";
constexpr const char* TANK_LEFT = "/assets/textures/tank walk/Zombie_Big_Side-left_Walk-Sheet8.png";

//NOWE WIEŻE
// 1. ZIELONY KARABIN (Machine Gun)
constexpr const char* MG_TOWER_LVL1 = "/assets/textures/green/turret_01_mk1.png";
constexpr const char* MG_TOWER_LVL2 = "/assets/textures/green/turret_01_mk2.png";
constexpr const char* MG_TOWER_LVL3 = "/assets/textures/green/turret_01_mk3.png";

// 2. ZŁOTY SNAJPER (Sniper) - Używamy jednej grafiki, ale w kodzie będziemy ją wydłużać!
constexpr const char* SNIPER_TOWER  = "/assets/textures/gold/turret_01_mk1.png";

// 3. FIOLETOWA WYRZUTNIA (Rocket)
constexpr const char* ROCKET_TOWER_LVL1 = "/assets/textures/purple/turret_03_mk1.png";
constexpr const char* ROCKET_TOWER_LVL2 = "/assets/textures/purple/turret_03_mk2.png";
constexpr const char* ROCKET_TOWER_LVL3 = "/assets/textures/purple/turret_03_mk3.png";

// 4. CZERWONY MIOTACZ OGNIA (Flamethrower) - Używamy masywnych luf
constexpr const char* FLAME_TOWER_LVL1 = "/assets/textures/red/turret_04_mk1.png";
constexpr const char* FLAME_TOWER_LVL2 = "/assets/textures/red/turret_04_mk2.png";
constexpr const char* FLAME_TOWER_LVL3 = "/assets/textures/red/turret_04_mk3.png";

// 5. NIEBIESKI SPOWALNIACZ (Slower) - Używamy luf z rdzeniem energetycznym
constexpr const char* SLOWER_TOWER_LVL1 = "/assets/textures/blue/turret_02_mk1.png";
constexpr const char* SLOWER_TOWER_LVL2 = "/assets/textures/blue/turret_02_mk2.png";
constexpr const char* SLOWER_TOWER_LVL3 = "/assets/textures/blue/turret_02_mk3.png";
/*
// Tekstury wieży karabinu maszynowego (folder wg nazwy nadanej przez użytkownika).
// Wieża ma tylko 2 poziomy grafiki — poziom 3 używa tekstury poziomu 2.
constexpr const char* MG_TOWER_LVL1 = "/assets/textures/machingunTower/Poziom_1.png";
constexpr const char* MG_TOWER_LVL2 = "/assets/textures/machingunTower/Poziom_2.png";

// Tekstury wyrzutni rakiet (3 poziomy) + tekstura pocisku-rakiety.
constexpr const char* ROCKET_TOWER_LVL1 = "/assets/textures/rocketTower/Poziom_1.png";
constexpr const char* ROCKET_TOWER_LVL2 = "/assets/textures/rocketTower/Poziom_2.png";
constexpr const char* ROCKET_TOWER_LVL3 = "/assets/textures/rocketTower/Poziom_3.png";*/
constexpr const char* ROCKET_PROJECTILE = "/assets/textures/rocketTower/Rakieta.png";

}

// Tekstury wież i rakiety narysowane są "lufą do góry". Pole rotation liczone jest
// atan2 (0° = w prawo), więc sprite obracamy o ten offset, by lufa celowała w cel.
constexpr float TOWER_TEXTURE_FORWARD = 90.f;

// Maksymalny dopuszczalny błąd kąta (w stopniach) między lufą a celem, przy którym
// wieża może wystrzelić. Powyżej tej tolerancji wieża najpierw musi się doobrócić.
constexpr float TOWER_AIM_TOLERANCE = 15.f;

// ===== DOSTĘPNE MAPY (do wyboru z menu) =====
// Aby dodać kolejną planszę: stwórz plik w assets/maps/, dopisz jego ścieżkę
// i nazwę poniżej oraz zwiększ COUNT. Reszta (wybór, ładowanie) działa sama.
namespace Maps {
constexpr int COUNT = 2;
// Ścieżki względem katalogu projektu (ASSETS_DIR doklejany przy ładowaniu).
constexpr const char* PATHS[COUNT] = {
    "/assets/maps/map1.txt",
    "/assets/maps/map2.txt"
};
// Nazwy wyświetlane w menu wyboru mapy.
constexpr const char* NAMES[COUNT] = {
    "Las",
    "Miasto"
};
}

// ===== GRACZ =====
constexpr int STARTING_LIVES = 20;
constexpr int STARTING_MONEY = 230;

// ===== ZOMBIE (statystyki bazowe) =====
namespace Walker {
constexpr int HP = 60;
constexpr float SPEED = 50.f;       // piksele/sekundę
constexpr int REWARD = 10;
constexpr int LIFECOST = 1;
}
namespace Runner {
constexpr int HP = 40;
constexpr float SPEED = 200.f;
constexpr int REWARD = 15;
constexpr int LIFECOST = 1;
}

namespace Armored {
constexpr int HP = 180;
constexpr float SPEED = 50.f;
constexpr int REWARD = 25;
constexpr float BULLET_RESISTANCE = 0.5f;   // 50% redukcja
constexpr int LIFECOST = 2;
}

namespace Tank {
constexpr int HP = 600;
constexpr float SPEED = 25.f;
constexpr int REWARD = 60;
constexpr int LIFECOST = 5;
}

// ===== WIEŻE =====
namespace MachineGunTower {
constexpr int COST = 50;
constexpr int DAMAGE = 5;
constexpr float RANGE = 150.f;
constexpr float FIRE_RATE = 4.f;            // strzałów/sekundę
constexpr float ROTATION_SPEED = 360.f;     // stopnie/sekundę
}

namespace SniperTower {
constexpr int COST = 100;
constexpr int DAMAGE = 50;
constexpr float RANGE = 400.f;
constexpr float FIRE_RATE = 0.5f;
constexpr float ROTATION_SPEED = 90.f;
}

namespace RocketTower {
constexpr int COST = 150;
constexpr int DAMAGE = 30;
constexpr float RANGE = 250.f;
constexpr float FIRE_RATE = 1.f;
constexpr float ROTATION_SPEED = 180.f;
constexpr float AOE_RADIUS = 60.f;
}

namespace FlamethrowerTower {
constexpr int COST = 80;
constexpr int DAMAGE_PER_SECOND = 15;
constexpr float RANGE = 120.f;
constexpr float FIRE_RATE = 5.f;
constexpr float BURN_DURATION = 2.f;
constexpr float ROTATION_SPEED = 230.f;
}

namespace SlowerTower {
constexpr int COST = 60;
constexpr float RANGE = 120.f;
constexpr float SLOW_FACTOR = 0.5f;         // mnożnik prędkości
constexpr float SLOW_DURATION = 1.f;
constexpr float FIRE_RATE = 1.5f;
constexpr float ROTATION_SPEED = 150.f;
}

// ===== POCISKI =====
constexpr float BULLET_SPEED = 600.f;
constexpr float ROCKET_SPEED = 400.f;
constexpr float FLAME_SPEED = 400.f;
constexpr float SNOWBALL_SPEED = 500.f;
constexpr float HIT_RADIUS = 25.f;              // promień kolizji

// ===== BONUSY =====
constexpr float BONUS_DROP_CHANCE = 0.08f;      // 8%
constexpr float BONUS_LIFETIME = 6.f;           // sekund
constexpr int AMMO_BONUS_VALUE = 30;            // dodatkowa waluta
constexpr int MEDKIT_BONUS_VALUE = 1;           // dodatkowe życie
constexpr float EMP_STUN_DURATION = 3.f;        // sekund
constexpr float BONUS_FALL_SPEED = 250.f; // Prędkość spadania paczki

// ===== FALE =====
constexpr float SPAWN_INTERVAL = 1.5f;          // sekund między zombi
constexpr int TOTAL_WAVES = 15;

// ===== ULEPSZENIA =====
constexpr float UPGRADE_DAMAGE_MULTIPLIER = 1.5f;
constexpr float UPGRADE_RANGE_MULTIPLIER = 1.2f;
constexpr float UPGRADE_COST_MULTIPLIER = 1.5f;
constexpr float SELL_REFUND = 0.7f;             // 70% zwrotu

}

#endif // CONFIG_H
