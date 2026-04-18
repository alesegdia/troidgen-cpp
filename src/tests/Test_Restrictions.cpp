#include "troidgen/restriction/RestrictionSet.hpp"
#include <cassert>
#include <iostream>

using namespace troidgen;

static RestrictionSet buildRestrictionFromNumber(int n) {
    return RestrictionSet(4, {(n & (1 << 0)) != 0,
                              (n & (1 << 1)) != 0,
                              (n & (1 << 2)) != 0,
                              (n & (1 << 3)) != 0});
}

int main() {
    for (int i = 0; i < 16; i++) {
        RestrictionSet baseRestriction = buildRestrictionFromNumber(i);
        std::cout << "br: " << baseRestriction.toString() << "\n";
        for (int j = 0; j < 16; j++) {
            RestrictionSet testRestriction = buildRestrictionFromNumber(j);
            std::cout << "\t(" << j << ")tr: " << testRestriction.toString() << "\n";
            if ((i & j) == j) {
                assert(testRestriction.resolves(baseRestriction));
            } else {
                assert(!testRestriction.resolves(baseRestriction));
            }
        }
    }
    std::cout << "Test_Restrictions: all assertions passed\n";
    return 0;
}
