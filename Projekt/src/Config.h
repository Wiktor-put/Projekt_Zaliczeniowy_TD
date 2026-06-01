#ifndef CONFIG_H
#define CONFIG_H

// Config.h — centralne stałe konfiguracyjne gry (okno, zombie, wieże, pociski, fale)

namespace Config {

// ===== OKNO I PĘTLA GRY =====
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int FPS_LIMIT = 60;

// ===== GRACZ =====
constexpr int STARTING_LIVES = 20;
constexpr int STARTING_MONEY = 200;

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
//constexpr int LIFECOST = 1;
}

namespace Armored {
constexpr int HP = 180;
constexpr float SPEED = 50.f;
constexpr int REWARD = 25;
constexpr float BULLET_RESISTANCE = 0.5f;   // 50% redukcja
//constexpr int LIFECOST = 2;
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
constexpr float RANGE = 100.f;
constexpr float FIRE_RATE = 5.f;
constexpr float BURN_DURATION = 2.f;
}

namespace SlowerTower {
constexpr int COST = 60;
constexpr float RANGE = 120.f;
constexpr float SLOW_FACTOR = 0.5f;         // mnożnik prędkości
constexpr float SLOW_DURATION = 1.f;
}

// ===== POCISKI =====
constexpr float BULLET_SPEED = 600.f;
constexpr float ROCKET_SPEED = 400.f;
constexpr float HIT_RADIUS = 25.f;              // promień kolizji

// ===== BONUSY =====
constexpr float BONUS_DROP_CHANCE = 0.08f;      // 8%
constexpr float BONUS_LIFETIME = 6.f;           // sekund
constexpr int AMMO_BONUS_VALUE = 30;            // dodatkowa waluta
constexpr int MEDKIT_BONUS_VALUE = 1;           // dodatkowe życie
constexpr float EMP_STUN_DURATION = 3.f;        // sekund

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
