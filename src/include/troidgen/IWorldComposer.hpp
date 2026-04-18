#pragma once
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class IWorldComposer {
public:
    virtual ~IWorldComposer() = default;
    virtual std::vector<std::shared_ptr<Room>> compose(
        const std::vector<std::shared_ptr<Room>>& fixed) = 0;
};

} // namespace troidgen
