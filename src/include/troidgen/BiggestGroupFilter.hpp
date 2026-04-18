#pragma once
#include "IRoomGroupFilter.hpp"

namespace troidgen {

class BiggestGroupFilter : public IRoomGroupFilter {
public:
    std::vector<std::shared_ptr<Room>> filter(
        const std::vector<std::vector<std::shared_ptr<Room>>>& groups) override;
};

} // namespace troidgen
