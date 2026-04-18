#pragma once
#include <random>
#include <memory>
#include <chrono>

namespace troidgen {

class RNG {
public:
    static std::unique_ptr<RNG> rng;

    std::mt19937 engine;

    RNG();

    // Returns a value in [0, n) — mirrors java.util.Random.nextInt(n)
    int nextInt(int n);

    // Returns a value in [min, max] inclusive — mirrors RNG.nextInt(min, max)
    int nextInt(int min, int max);

    float nextFloat();
};

} // namespace troidgen
