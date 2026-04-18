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
    rects.push_back(std::make_shared<Room>(0.0f, 0.0f, 3.0f, 3.0f));
    rects.push_back(std::make_shared<Room>(2.0f, 2.0f, 4.0f, 4.0f));
    rects.push_back(std::make_shared<Room>(1.0f, 2.0f, 2.0f, 2.0f));
    rects.push_back(std::make_shared<Room>(3.0f, 2.0f, 3.0f, 3.0f));
    rects.push_back(std::make_shared<Room>(-1.0f, 2.0f, 2.0f, 2.0f));

    std::cout << "Before solve:\n";
    for (const auto& r : rects) std::cout << "  " << r->toString() << "\n";

    OverlapSolver os;
    OverlapSolverConfig osc;
    rects = os.solve(osc, rects);

    std::cout << "After solve:\n";
    for (const auto& r : rects) std::cout << "  " << r->toString() << "\n";

    return 0;
}
