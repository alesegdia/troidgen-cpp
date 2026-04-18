#include "troidgen/restriction/RestrictionSetFactory.hpp"

namespace troidgen {

RestrictionSet RestrictionSetFactory::makeRS_0000() {
    return RestrictionSet(NUM_RESTRICTIONS, {false, false, false, false});
}

RestrictionSet RestrictionSetFactory::makeRS_1000() {
    return RestrictionSet(NUM_RESTRICTIONS, {true, false, false, false});
}

RestrictionSet RestrictionSetFactory::makeRS_0100() {
    return RestrictionSet(NUM_RESTRICTIONS, {false, true, false, false});
}

RestrictionSet RestrictionSetFactory::makeRS_1010() {
    return RestrictionSet(NUM_RESTRICTIONS, {true, false, true, false});
}

} // namespace troidgen
