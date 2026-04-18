#pragma once
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class LinkBuilder {
public:
    void generate(const std::vector<std::shared_ptr<Room>>& rooms);
};

} // namespace troidgen
