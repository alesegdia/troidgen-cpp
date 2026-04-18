#include "troidgen/util/RNG.hpp"
#include <chrono>

namespace troidgen {

std::unique_ptr<RNG> RNG::rng;

RNG::RNG() {
    engine.seed(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));
}

int RNG::nextInt(int n) {
    std::uniform_int_distribution<int> dist(0, n - 1);
    return dist(engine);
}

int RNG::nextInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

float RNG::nextFloat() {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(engine);
}

} // namespace troidgen
