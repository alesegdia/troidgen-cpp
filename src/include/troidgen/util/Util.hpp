#pragma once
#include "RNG.hpp"
#include <vector>
#include <algorithm>

namespace troidgen {

class Util {
public:
    template<typename T>
    static void shuffle(std::vector<T>& list) {
        for (int i = 0; i < static_cast<int>(list.size()); i++) {
            int i1 = RNG::rng->nextInt(static_cast<int>(list.size()));
            int i2 = RNG::rng->nextInt(static_cast<int>(list.size()));
            std::swap(list[i1], list[i2]);
        }
    }
};

} // namespace troidgen
