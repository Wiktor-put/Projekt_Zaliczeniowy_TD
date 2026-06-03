#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include "Gameobject.h"
#include <vector>
#include <memory>
#include <random>
#include <string>

// Typ zombi dla spawnu (enum używany tylko przez WaveManager)
enum class ZombieType {
    WALKER,
    RUNNER,
    ARMORED,
    TANK
};

// Pojedyncza para "typ -> ilość" w fali
struct WaveEntry {
    ZombieType type;
    int count;
};

class WaveManager {
private:
    // Każda fala to lista WaveEntry. waves[i] to definicja fali numer i+1.
    std::vector<std::vector<WaveEntry>> waves;

    int currentWaveIndex;        // -1 = nie rozpoczęto, 0 = pierwsza fala
    bool waveInProgress;
    float spawnTimer;
    float spawnInterval;

    // Lista zombi do zespawnowania w obecnej fali (rozwinięta z WaveEntry)
    // np. [WALKER, WALKER, RUNNER, WALKER, ARMORED, ...]
    std::vector<ZombieType> spawnQueue;
    int spawnedCount;            // ile już zespawnowano

public:
    WaveManager();

    // Wczytuje konfigurację fal z pliku tekstowego (format niżej)
    void loadFromFile(const std::string& path);

    // Rozpoczyna następną falę (gracz wciska SPACJĘ lub przycisk)
    void startNextWave();

    // Spawnuje zombi z aktualnej fali w odstępach spawnInterval
    void update(float dt, std::vector<std::unique_ptr<GameObject>>& objects,
                const std::vector<sf::Vector2f>& waypoints);

    bool isWaveInProgress() const { return waveInProgress; }
    bool allWavesFinished() const;
    int getCurrentWaveNumber() const { return currentWaveIndex + 1; }
    int getTotalWaves() const { return static_cast<int>(waves.size()); }

    // Reset do nowej gry
    void reset();

    //do zapisu
    void setCurrentWaveIndex(int index) { currentWaveIndex = index; }
    int getCurrentWaveIndex() const { return currentWaveIndex; }
};

#endif
