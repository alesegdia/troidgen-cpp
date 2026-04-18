#pragma once
#include "IRoomProvider.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class ManualRoomProvider : public IRoomProvider {
public:
    void add(std::shared_ptr<Room> mg);
    void addAll(const std::vector<std::shared_ptr<Room>>& rooms);

    std::vector<std::shared_ptr<Room>> provideList(int numRooms) override;
    void notifySelected(const std::vector<std::shared_ptr<Room>>& rooms) override {}
    bool isOk() override { return false; }

private:
    std::vector<std::shared_ptr<Room>> availableGeom;
};

} // namespace troidgen
