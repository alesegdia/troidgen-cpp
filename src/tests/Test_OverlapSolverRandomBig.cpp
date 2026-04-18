#include "troidgen/OverlapSolver.hpp"
#include "troidgen/OverlapSolverConfig.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/room/Room.hpp"
#include <iostream>
#include <memory>
#include <vector>

using namespace troidgen;

int main() {
    RNG::rng = std::make_unique<RNG>();

    std::vector<std::shared_ptr<Room>> rects;
    for (int i = 0; i < 200; i++) {
        int x = RNG::rng->nextInt(-20, 20);
        int y = RNG::rng->nextInt(-20, 20);
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
    osc.enclosingRect.size.x     += 200.0f;
    osc.enclosingRect.size.y     += 200.0f;
    osc.enclosingRect.position.x -= 100.0f;
    osc.enclosingRect.position.y -= 100.0f;

    rects = os.solve(osc, rects);

    std::cout << "Test_OverlapSolverRandomBig: solved " << rects.size() << " rooms\n";
    return 0;
}
