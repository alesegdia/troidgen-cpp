#pragma once
#include "troidgen/room/Room.hpp"

namespace troidgen {

class IRoomValidator {
public:
    virtual ~IRoomValidator() = default;
    virtual bool validate(Room* r) = 0;
};

} // namespace troidgen
