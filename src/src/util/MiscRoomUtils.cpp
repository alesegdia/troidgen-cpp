#include "troidgen/util/MiscRoomUtils.hpp"
#include "troidgen/util/RNG.hpp"

namespace troidgen {

void MiscRoomUtils::RandomPlaceInRange(std::vector<std::shared_ptr<Room>>& rooms,
                                       const Rect& enclosing) {
    for (auto& r : rooms) {
        r->position.x = static_cast<float>(RNG::rng->nextInt(
            static_cast<int>(enclosing.position.x),
            static_cast<int>(enclosing.position.x + enclosing.size.x)));
        r->position.y = static_cast<float>(RNG::rng->nextInt(
            static_cast<int>(enclosing.position.y),
            static_cast<int>(enclosing.position.y + enclosing.size.y)));
    }
}

} // namespace troidgen
