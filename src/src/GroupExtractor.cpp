#include "troidgen/GroupExtractor.hpp"
#include <algorithm>

namespace troidgen {

static bool containsRoom(const std::vector<std::shared_ptr<Room>>& list,
                         const std::shared_ptr<Room>& item) {
    return std::find(list.begin(), list.end(), item) != list.end();
}

std::vector<std::vector<std::shared_ptr<Room>>> GroupExtractor::solve(
    const std::vector<std::shared_ptr<Room>>& testRects) {
    return solve(testRects, nullptr);
}

std::vector<std::vector<std::shared_ptr<Room>>> GroupExtractor::solve(
    const std::vector<std::shared_ptr<Room>>& testRects,
    IRoomValidator* rgv) {

    std::vector<std::vector<std::shared_ptr<Room>>> rectGroups;
    std::vector<std::shared_ptr<Room>> assignedRooms;

    while (assignedRooms.size() < testRects.size()) {
        for (const auto& outer_room : testRects) {
            if (!containsRoom(assignedRooms, outer_room)) {
                if (rgv == nullptr || rgv->validate(outer_room.get())) {
                    std::vector<std::shared_ptr<Room>> currentGroup;
                    currentGroup.push_back(outer_room);
                    assignedRooms.push_back(outer_room);
                    collectConnected(testRects, assignedRooms, currentGroup, outer_room, rgv);
                    rectGroups.push_back(std::move(currentGroup));
                }
                if (rgv != nullptr && !rgv->validate(outer_room.get()) &&
                    !containsRoom(assignedRooms, outer_room)) {
                    assignedRooms.push_back(outer_room);
                }
            }
        }
    }
    return rectGroups;
}

void GroupExtractor::collectConnected(
    const std::vector<std::shared_ptr<Room>>& testRects,
    std::vector<std::shared_ptr<Room>>& assignedRooms,
    std::vector<std::shared_ptr<Room>>& currentGroup,
    const std::shared_ptr<Room>& r,
    IRoomValidator* rgv) {

    for (const auto& inner_room : testRects) {
        if (rgv == nullptr || rgv->validate(inner_room.get())) {
            if (r.get() != inner_room.get() &&
                !containsRoom(assignedRooms, inner_room) &&
                r->isTouching(*inner_room) &&
                !r->getPossibleConnections(inner_room.get()).empty() &&
                !inner_room->collideWith(*r)) {

                currentGroup.push_back(inner_room);
                assignedRooms.push_back(inner_room);
                collectConnected(testRects, assignedRooms, currentGroup, inner_room, rgv);
            }
        }
        if (rgv != nullptr && !rgv->validate(inner_room.get()) &&
            !containsRoom(assignedRooms, inner_room)) {
            assignedRooms.push_back(inner_room);
        }
    }
}

} // namespace troidgen
