#pragma once
#include "troidgen/util/Rect.hpp"

namespace troidgen {

struct OverlapSolverConfig {
    float separationParameter   = 1.0f;
    int   resolution            = 64;
    bool  interactive           = false;
    bool  enableRandomDisplacement   = true;
    bool  enableRandomPushIfZero     = true;
    bool  enableTweakNearSeparation  = true;
    Rect  enclosingRect{-400.0f, -400.0f, 800.0f, 800.0f};
    bool  earlyBreak = false;

    float enclosingLeft()   const { return enclosingRect.position.x * resolution; }
    float enclosingRight()  const { return (enclosingRect.position.x + enclosingRect.size.x) * resolution; }
    float enclosingTop()    const { return enclosingRect.position.y * resolution; }
    float enclosingBottom() const { return (enclosingRect.position.y + enclosingRect.size.y) * resolution; }
};

} // namespace troidgen
