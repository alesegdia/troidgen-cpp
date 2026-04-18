#include "troidgen/room/RoomInfo.hpp"
#include <sstream>

namespace troidgen {

std::string RoomInfo::toString() const {
    std::ostringstream oss;
    oss << "[" << roomTypeToString(rtype);
    if (rtype == RoomType::ABILITY) {
        oss << ",\n\tsolved(" << constraintSolved << ")";
    }
    if (rtype == RoomType::PASSAGE) {
        oss << ",\n\tneeded(" << passageNextRestriction.toString() << ")";
    }
    oss << ",\n\tsize(" << size.x << "," << size.y << "),\n";
    for (const auto& l : linkInfo) {
        oss << "\t(" << directionToString(l.direction)
            << " (" << l.relativeCoordinate.x << "," << l.relativeCoordinate.y << ")),\n";
    }
    oss << "]";
    return oss.str();
}

} // namespace troidgen
