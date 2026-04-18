#include "troidgen/GroupExtractor.hpp"
#include "troidgen/OverlapSolver.hpp"
#include "troidgen/OverlapSolverConfig.hpp"
#include "troidgen/RandomRoomProvider.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/util/MiscRoomUtils.hpp"
#include "troidgen/room/Room.hpp"
#include <iostream>
#include <memory>
#include <vector>

using namespace troidgen;

int main() {
    RNG::rng = std::make_unique<RNG>();

    RandomRoomProvider geoProv(1, 4);
    auto rects = geoProv.provideList(20);
    MiscRoomUtils::RandomPlaceInRange(rects, Rect(-4, -4, 8, 8));

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
