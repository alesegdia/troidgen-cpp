#pragma once
#include <string>

namespace troidgen {

enum class Direction {
    TOP,
    DOWN,
    LEFT,
    RIGHT,
    NODIR
};

Direction getDirectionFromChar(char c);
std::string directionToString(Direction d);

} // namespace troidgen
