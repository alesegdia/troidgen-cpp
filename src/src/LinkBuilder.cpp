#include "troidgen/LinkBuilder.hpp"
#include "troidgen/util/RNG.hpp"
#include <cassert>

namespace troidgen {

void LinkBuilder::generate(const std::vector<std::shared_ptr<Room>>& rooms) {
    for (const auto& r1 : rooms) {
        for (Room* r2 : r1->neighboors) {
            if (r1.get() != r2 && r1->isTouching(*r2) && !r1->directlyConnectedWith(r2)) {
                auto possibleLinks = r1->getPossibleConnections(r2);
                if (!possibleLinks.empty()) {
                    int idx = RNG::rng->nextInt(0, static_cast<int>(possibleLinks.size()) - 1);
                    auto& lp = possibleLinks[static_cast<size_t>(idx)];
                    lp.connect();
                    assert(lp.lA->isConnected());
                    assert(lp.lB->isConnected());
                }
            }
        }
    }
}

} // namespace troidgen
