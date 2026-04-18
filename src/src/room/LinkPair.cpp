#include "troidgen/room/LinkPair.hpp"
#include "troidgen/room/Link.hpp"

namespace troidgen {

void LinkPair::connect() {
    lA->connectTo(lB);
}

} // namespace troidgen
