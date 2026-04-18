#pragma once
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class IRoomGroupValidator {
public:
    virtual ~IRoomGroupValidator() = default;
    virtual bool validate(const std::vector<std::shared_ptr<Room>>& outputLayout) = 0;
};

} // namespace troidgen
