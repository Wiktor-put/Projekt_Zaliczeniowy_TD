#ifndef RANDOM_H
#define RANDOM_H

// Random.h — helper do generowania liczb losowych (Mersenne Twister, jeden generator)

#include <random>

namespace Random {

// Globalny generator - inicjalizowany raz, używany wielokrotnie
inline std::mt19937& engine() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

// Losowy int z zakresu [min, max] (włącznie)
inline int intInRange(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine());
}

// Losowy float z zakresu [min, max)
inline float floatInRange(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine());
}

// Zwraca true z zadanym prawdopodobieństwem (0.0 - 1.0)
inline bool chance(float probability) {
    return floatInRange(0.f, 1.f) < probability;
}
}

#endif // RANDOM_H
