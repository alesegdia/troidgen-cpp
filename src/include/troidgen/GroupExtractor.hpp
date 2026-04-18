#pragma once
#include "IRoomValidator.hpp"
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class GroupExtractor {
public:
    std::vector<std::vector<std::shared_ptr<Room>>> solve(
        const std::vector<std::shared_ptr<Room>>& testRects);

    std::vector<std::vector<std::shared_ptr<Room>>> solve(
        const std::vector<std::shared_ptr<Room>>& testRects,
        IRoomValidator* rgv);

private:
    void collectConnected(
        const std::vector<std::shared_ptr<Room>>& testRects,
        std::vector<std::shared_ptr<Room>>& assignedRooms,
        std::vector<std::shared_ptr<Room>>& currentGroup,
        const std::shared_ptr<Room>& r,
        IRoomValidator* rgv);
};

} // namespace troidgen
