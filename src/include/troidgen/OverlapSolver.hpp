#pragma once
#include "OverlapSolverConfig.hpp"
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class OverlapSolver {
public:
    std::vector<std::shared_ptr<Room>> solve(
        const OverlapSolverConfig& osc,
        const std::vector<std::shared_ptr<Room>>& testRects);

    std::vector<std::shared_ptr<Room>> solve(
        const OverlapSolverConfig& osc,
        const std::vector<std::shared_ptr<Room>>& testRects,
        const std::vector<std::shared_ptr<Room>>& outputLayout);

private:
    static float nonZeroRnd(int min, int max);
    static void repulse(Rect& r, Vec2& push, float separationParameter);
};

} // namespace troidgen
