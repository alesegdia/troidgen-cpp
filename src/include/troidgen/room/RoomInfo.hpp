#pragma once
#include "LinkInfo.hpp"
#include "RoomType.hpp"
#include "troidgen/restriction/RestrictionSet.hpp"
#include "troidgen/util/Vec2.hpp"
#include <vector>
#include <string>

namespace troidgen {

struct RoomInfo {
    Vec2 size;
    std::vector<LinkInfo> linkInfo;
    RestrictionSet restriction;
    RoomType rtype = RoomType::COMMON;
    RestrictionSet passageNextRestriction;
    int constraintSolved = 0;
    std::string id;
    int neededRooms = 5;

    std::string toString() const;
};

} // namespace troidgen
