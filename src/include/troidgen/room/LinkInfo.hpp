#pragma once
#include "Direction.hpp"
#include "troidgen/util/Vec2.hpp"

namespace troidgen {

struct LinkInfo {
    Vec2 relativeCoordinate;
    Direction direction;

    LinkInfo(float x, float y, Direction dir)
        : relativeCoordinate(x, y), direction(dir) {}
};

} // namespace troidgen
