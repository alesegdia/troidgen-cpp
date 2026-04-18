#include "troidgen/room/RoomType.hpp"

namespace troidgen {

RoomType roomTypeFromString(const std::string& s) {
    if (s == "common")  return RoomType::COMMON;
    if (s == "passage") return RoomType::PASSAGE;
    if (s == "ability") return RoomType::ABILITY;
    return RoomType::COMMON;
}

std::string roomTypeToString(RoomType rt) {
    switch (rt) {
        case RoomType::PASSAGE: return "PASSAGE";
        case RoomType::ABILITY: return "ABILITY";
        default:                return "COMMON";
    }
}

} // namespace troidgen
