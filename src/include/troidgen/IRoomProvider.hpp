#pragma once
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class IRoomProvider {
public:
    virtual ~IRoomProvider() = default;
    virtual std::vector<std::shared_ptr<Room>> provideList(int numRooms) = 0;
    virtual void notifySelected(const std::vector<std::shared_ptr<Room>>& rooms) = 0;
    virtual bool isOk() = 0;
};

} // namespace troidgen
