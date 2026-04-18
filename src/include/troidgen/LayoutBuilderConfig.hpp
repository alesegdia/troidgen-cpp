#pragma once
#include "OverlapSolverConfig.hpp"
#include "troidgen/util/Rect.hpp"

namespace troidgen {

struct LayoutBuilderConfig {
    int  numRooms      = 20;
    int  numIterations = 5;
    Rect spawnRect{-4.0f, -4.0f, 8.0f, 8.0f};
    OverlapSolverConfig osc;
    bool interactive   = true;  // show RectDebugger window each generation pass
};

} // namespace troidgen
