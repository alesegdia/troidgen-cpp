#include "troidgen/OverlapSolver.hpp"
#include "troidgen/OverlapSolverConfig.hpp"
#include "troidgen/GroupExtractor.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/room/Room.hpp"
#include <iostream>
#include <memory>
#include <vector>

using namespace troidgen;

int main() {
    RNG::rng = std::make_unique<RNG>();

    std::vector<std::shared_ptr<Room>> rects;
    for (int i = 0; i < 20; i++) {
        int x = RNG::rng->nextInt(-4, 4);
        int y = RNG::rng->nextInt(-4, 4);
        int w = RNG::rng->nextInt(1, 3);
        int h = RNG::rng->nextInt(1, 3);
        rects.push_back(std::make_shared<Room>(
            static_cast<float>(x), static_cast<float>(y),
            static_cast<float>(w), static_cast<float>(h)));
    }

    OverlapSolver os;
    OverlapSolverConfig osc;
    osc.separationParameter = 1.0f;
    osc.enableTweakNearSeparation = false;

    rects = os.solve(osc, rects);

    GroupExtractor ge;
    auto groups = ge.solve(rects);

    const std::vector<std::shared_ptr<Room>>* best = &groups[0];
    for (const auto& group : groups) {
        if (group.size() > best->size()) best = &group;
        std::cout << "size: " << group.size() << "\n";
        for (const auto& r : group) std::cout << "  " << r->toString() << "\n";
        std::cout << "========================\n";
    }

    std::cout << "Best group size: " << best->size() << "\n";
    return 0;
}
