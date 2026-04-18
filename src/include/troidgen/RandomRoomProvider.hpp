#pragma once
#include "IRoomProvider.hpp"

namespace troidgen {

class RandomRoomProvider : public IRoomProvider {
public:
    RandomRoomProvider(int min, int max) : min(min), max(max) {}

    std::shared_ptr<Room> createRandomRoom();
    std::vector<std::shared_ptr<Room>> provideList(int numRooms) override;
    void notifySelected(const std::vector<std::shared_ptr<Room>>& rooms) override {}
    bool isOk() override { return false; }

private:
    int min;
    int max;
};

} // namespace troidgen
