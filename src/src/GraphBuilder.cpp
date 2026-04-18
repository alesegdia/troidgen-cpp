#include "troidgen/GraphBuilder.hpp"
#include <limits>

namespace troidgen {

UpperMatrix2D<float> GraphBuilder::build(
    const std::vector<std::shared_ptr<Room>>& rects) {

    int n = static_cast<int>(rects.size());
    UpperMatrix2D<float> m(n, n, std::numeric_limits<float>::max());

    for (int oi = 0; oi < n; oi++) {
        for (int ii = 0; ii < n; ii++) {
            auto& router = rects[static_cast<size_t>(oi)];
            auto& rinner = rects[static_cast<size_t>(ii)];
            if (router.get() != rinner.get() && router->isTouching(*rinner)) {
                router->neighboors.push_back(rinner.get());
                m.Set(oi, ii, router->position.distance(rinner->position));
            }
        }
    }
    return m;
}

} // namespace troidgen
