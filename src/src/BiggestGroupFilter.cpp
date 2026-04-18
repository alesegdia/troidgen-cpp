#include "troidgen/BiggestGroupFilter.hpp"

namespace troidgen {

std::vector<std::shared_ptr<Room>> BiggestGroupFilter::filter(
    const std::vector<std::vector<std::shared_ptr<Room>>>& groups) {
    const std::vector<std::shared_ptr<Room>>* best = &groups[0];
    for (const auto& group : groups) {
        if (group.size() > best->size()) {
            best = &group;
        }
    }
    return *best;
}

} // namespace troidgen
