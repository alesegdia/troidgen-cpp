#include "troidgen/ManualRoomProvider.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/util/Util.hpp"
#include <cassert>

namespace troidgen {

void ManualRoomProvider::add(std::shared_ptr<Room> mg) {
    availableGeom.push_back(std::move(mg));
}

void ManualRoomProvider::addAll(const std::vector<std::shared_ptr<Room>>& rooms) {
    for (const auto& r : rooms) {
        add(r);
    }
}

std::vector<std::shared_ptr<Room>> ManualRoomProvider::provideList(int numRooms) {
    std::vector<std::shared_ptr<Room>> feasibleRooms = availableGeom;

    assert(!feasibleRooms.empty());
    Util::shuffle(feasibleRooms);

    std::vector<std::shared_ptr<Room>> retlist;
    for (int i = 0; i < numRooms; i++) {
        int maxIdx = static_cast<int>(feasibleRooms.size()) - 1;
        assert(maxIdx >= 0);
        // Return copies of template rooms so each call gets fresh unconnected rooms
        retlist.push_back(std::make_shared<Room>(*feasibleRooms[
            static_cast<size_t>(RNG::rng->nextInt(0, maxIdx))]));
    }
    return retlist;
}

} // namespace troidgen
