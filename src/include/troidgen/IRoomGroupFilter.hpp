#pragma once
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class IRoomGroupFilter {
public:
    virtual ~IRoomGroupFilter() = default;
    virtual std::vector<std::shared_ptr<Room>> filter(
        const std::vector<std::vector<std::shared_ptr<Room>>>& groups) = 0;
};

} // namespace troidgen
