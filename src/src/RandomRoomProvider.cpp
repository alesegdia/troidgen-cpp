#include "troidgen/RandomRoomProvider.hpp"
#include "troidgen/util/RNG.hpp"

namespace troidgen {

std::shared_ptr<Room> RandomRoomProvider::createRandomRoom() {
    return std::make_shared<Room>(
        static_cast<float>(RNG::rng->nextInt(min, max)),
        static_cast<float>(RNG::rng->nextInt(min, max)),
        nullptr);
}

std::vector<std::shared_ptr<Room>> RandomRoomProvider::provideList(int numRooms) {
    std::vector<std::shared_ptr<Room>> rects;
    for (int i = 0; i < numRooms; i++) {
        rects.push_back(createRandomRoom());
    }
    return rects;
}

} // namespace troidgen
