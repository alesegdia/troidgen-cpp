#include "troidgen/ExactRoomProvider.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/util/Util.hpp"
#include <cassert>

namespace troidgen {

void ExactRoomProvider::add(std::shared_ptr<Room> mg) {
    assert(mg->rinfo != nullptr);
    availableGeom.push_back(mg);
    totalNeededCount += mg->rinfo->neededRooms;
    if (neededRooms.find(mg->rinfo) == neededRooms.end()) {
        neededRooms[mg->rinfo] = mg->rinfo->neededRooms;
    }
}

void ExactRoomProvider::addAll(const std::vector<std::shared_ptr<Room>>& rooms) {
    for (const auto& r : rooms) {
        add(r);
    }
}

std::vector<std::shared_ptr<Room>> ExactRoomProvider::provideList(int /*numRooms*/) {
    std::vector<std::shared_ptr<Room>> feasibleRooms = availableGeom;
    assert(!feasibleRooms.empty());
    Util::shuffle(feasibleRooms);

    std::vector<std::shared_ptr<Room>> retlist;

    if (lastCount.has_value()) {
        isOkFlag = true;
        for (auto& [ri, needed] : neededRooms) {
            int present = 0;
            auto it = lastCount->find(ri);
            if (it != lastCount->end()) {
                present = it->second;
            }
            int diff = needed - present;
            for (int i = 0; i < diff; i++) {
                isOkFlag = false;
                retlist.push_back(std::make_shared<Room>(ri));
            }
        }
    } else {
        for (auto& [ri, count] : neededRooms) {
            for (int i = 0; i < count; i++) {
                retlist.push_back(std::make_shared<Room>(ri));
            }
        }
    }
    Util::shuffle(retlist);
    return retlist;
}

void ExactRoomProvider::notifySelected(const std::vector<std::shared_ptr<Room>>& rooms) {
    lastCount = std::unordered_map<RoomInfo*, int>{};
    for (const auto& r : rooms) {
        if (r->rinfo) {
            (*lastCount)[r->rinfo]++;
        }
    }
}

} // namespace troidgen
