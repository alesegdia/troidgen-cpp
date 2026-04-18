#pragma once
#include "IRoomProvider.hpp"
#include "troidgen/room/RoomInfo.hpp"
#include <unordered_map>
#include <optional>

namespace troidgen {

class ExactRoomProvider : public IRoomProvider {
public:
    void add(std::shared_ptr<Room> mg);
    void addAll(const std::vector<std::shared_ptr<Room>>& rooms);

    int totalNeeded() const { return totalNeededCount; }

    std::vector<std::shared_ptr<Room>> provideList(int numRooms) override;
    void notifySelected(const std::vector<std::shared_ptr<Room>>& rooms) override;
    bool isOk() override { return isOkFlag; }

private:
    std::vector<std::shared_ptr<Room>> availableGeom;
    std::unordered_map<RoomInfo*, int> neededRooms;
    std::optional<std::unordered_map<RoomInfo*, int>> lastCount;
    bool isOkFlag = false;
    int totalNeededCount = 0;
};

} // namespace troidgen
