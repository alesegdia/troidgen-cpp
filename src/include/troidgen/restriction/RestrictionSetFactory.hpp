#pragma once
#include "RestrictionSet.hpp"

namespace troidgen {

class RestrictionSetFactory {
public:
    static constexpr int NUM_RESTRICTIONS = 4;

    static RestrictionSet makeRS_0000();
    static RestrictionSet makeRS_1000();
    static RestrictionSet makeRS_0100();
    static RestrictionSet makeRS_1010();
};

} // namespace troidgen
