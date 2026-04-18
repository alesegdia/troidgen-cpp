#pragma once
#include <string>

namespace troidgen {

enum class RoomType {
    PASSAGE,
    ABILITY,
    COMMON
};

RoomType roomTypeFromString(const std::string& s);
std::string roomTypeToString(RoomType rt);

} // namespace troidgen
