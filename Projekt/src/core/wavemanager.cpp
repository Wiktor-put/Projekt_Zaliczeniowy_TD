#include "wavemanager.h"
#include "Random.h"
#include <fstream>
#include <sstream>
#include "walker.h"
#include "runner.h"
#include "armored.h"
#include "tank.h"
#include "Config.h"


WaveManager::WaveManager() {}

void WaveManager::loadFromFile(const std::string& path) {
    waves.clear();

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc " << path << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        // Parsowanie "wave 1: walker:10 runner:5"
        std::vector<WaveEntry> wave;
        std::istringstream iss(line);
        std::string token;

        iss >> token;  // "wave"
        iss >> token;  // "1:" - pomijamy

        while (iss >> token) {
            // token to "typ:ilosc", np. "walker:10"
            size_t colon = token.find(':');
            if (colon == std::string::npos) continue;

            std::string typeName = token.substr(0, colon);
            int count = std::stoi(token.substr(colon + 1));

            ZombieType type;
            if (typeName == "walker") type = ZombieType::WALKER;
            else if (typeName == "runner") type = ZombieType::RUNNER;
            else if (typeName == "armored") type = ZombieType::ARMORED;
            else if (typeName == "tank") type = ZombieType::TANK;
            else continue;

            wave.push_back({type, count});
        }

        if (!wave.empty()) waves.push_back(wave);
    }
}

std::vector<WaveEntry> WaveManager::generateRandomWave(int waveNumber) const {
    // Liczebność każdego typu skalujemy numerem fali, by trudność dalej rosła,
    // a losowe widełki dają zmienność (każda fala inna). Wartości dobrane tak,
    // by płynnie kontynuować trudność po ostatniej fali z pliku (fala 20).
    std::vector<WaveEntry> wave;

    int walkers = Random::intInRange(waveNumber * 3, waveNumber * 4);
    int runners = Random::intInRange(waveNumber * 2, waveNumber * 3);
    int armored = Random::intInRange(waveNumber * 3 / 2, waveNumber * 2);
    int tanks   = Random::intInRange(waveNumber, waveNumber * 3 / 2);

    // Dodajemy tylko niepuste wpisy (zerowa liczebność nie ma sensu).
    if (walkers > 0) wave.push_back({ZombieType::WALKER, walkers});
    if (runners > 0) wave.push_back({ZombieType::RUNNER, runners});
    if (armored > 0) wave.push_back({ZombieType::ARMORED, armored});
    if (tanks   > 0) wave.push_back({ZombieType::TANK,    tanks});

    return wave;
}

void WaveManager::startNextWave() {
    currentWaveIndex++;

    // Pierwsze fale czytamy z pliku; po ich wyczerpaniu przechodzimy w tryb
    // nieskończony i generujemy kolejne fale losowo (gra trwa, aż gracz przegra).
    std::vector<WaveEntry> wave;
    if (currentWaveIndex < static_cast<int>(waves.size())) {
        wave = waves[currentWaveIndex];
    } else {
        wave = generateRandomWave(currentWaveIndex + 1);
    }

    // Rozwiń WaveEntry na pełną listę typów
    spawnQueue.clear();
    for (const auto& entry : wave) {
        for (int i = 0; i < entry.count; ++i) {
            spawnQueue.push_back(entry.type);
        }
    }

    // Losowo tasujemy kolejność spawnu, by każda fala przebiegała inaczej.
    std::shuffle(spawnQueue.begin(), spawnQueue.end(), Random::engine());

    spawnedCount = 0;
    spawnTimer = 0.f;
    waveInProgress = true;

    std::cout << "Fala " << (currentWaveIndex + 1) << " - "
              << spawnQueue.size() << " zombi" << std::endl;
}

void WaveManager::update(float dt,
                         std::vector<std::unique_ptr<GameObject>>& objects,
                         const std::vector<sf::Vector2f>& waypoints) {
    if (!waveInProgress) return;

    spawnTimer += dt;
    if (spawnTimer < spawnInterval) return;
    spawnTimer = 0.f;

    // Czy są jeszcze zombi do zespawnowania?
    if (spawnedCount >= static_cast<int>(spawnQueue.size())) {
        waveInProgress = false;
        std::cout << "Fala " << (currentWaveIndex + 1) << " zespawnowana" << std::endl;
        return;
    }

    // Zespawnuj kolejnego zombi
    ZombieType type = spawnQueue[spawnedCount];
    switch (type) {
    case ZombieType::WALKER:
        objects.push_back(std::make_unique<Walker>(waypoints)); break;
    case ZombieType::RUNNER:
        objects.push_back(std::make_unique<Runner>(waypoints)); break;
    case ZombieType::ARMORED:
        objects.push_back(std::make_unique<Armored>(waypoints)); break;
    case ZombieType::TANK:
        objects.push_back(std::make_unique<Tank>(waypoints)); break;
    }
    spawnedCount++;
}

void WaveManager::reset(){
    waves = {};

    currentWaveIndex = -1;
    waveInProgress = false;;
    spawnTimer = 0.f;
    spawnInterval = Config::SPAWN_INTERVAL;

    spawnQueue = {};
    spawnedCount = 0;
}
