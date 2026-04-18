#pragma once
#include "troidgen/room/Room.hpp"
#include "troidgen/util/UpperMatrix2D.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class GraphBuilder {
public:
    UpperMatrix2D<float> build(const std::vector<std::shared_ptr<Room>>& rects);
};

} // namespace troidgen
