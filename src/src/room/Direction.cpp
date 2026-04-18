#include "troidgen/room/Direction.hpp"

namespace troidgen {

Direction getDirectionFromChar(char c) {
    switch (c) {
        case 'T': return Direction::TOP;
        case 'B': return Direction::DOWN;
        case 'L': return Direction::LEFT;
        case 'R': return Direction::RIGHT;
        default:  return Direction::NODIR;
    }
}

std::string directionToString(Direction d) {
    switch (d) {
        case Direction::TOP:   return "TOP";
        case Direction::DOWN:  return "DOWN";
        case Direction::LEFT:  return "LEFT";
        case Direction::RIGHT: return "RIGHT";
        default:               return "NODIR";
    }
}

} // namespace troidgen
