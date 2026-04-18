#pragma once
#include "troidgen/room/Room.hpp"
#include "troidgen/util/Rect.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class MiscRoomUtils {
public:
    static void RandomPlaceInRange(std::vector<std::shared_ptr<Room>>& rooms,
                                   const Rect& enclosing);
};

} // namespace troidgen
