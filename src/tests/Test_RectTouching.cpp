#include "troidgen/util/Rect.hpp"
#include <iostream>

using namespace troidgen;

int main() {
    Rect rr(0, 0, 3, 3);

    Rect r1(-3, 3, 3, 3);
    Rect r2(3,  3, 3, 3);
    Rect r3(-3, -3, 3, 3);
    Rect r4(3,  -3, 3, 3);

    std::cout << "TL: " << rr.isTouching(r1) << "\n";  // false (corner)
    std::cout << "BR: " << rr.isTouching(r2) << "\n";  // false (corner)
    std::cout << "BL: " << rr.isTouching(r3) << "\n";  // false (corner)
    std::cout << "TR: " << rr.isTouching(r4) << "\n";  // false (corner)

    Rect r5(-3, 0, 3, 3);
    std::cout << "r5 touching: " << rr.isTouching(r5) << "\n";  // true
    std::cout << "r5 collidin: " << rr.collideWith(r5) << "\n"; // false
    return 0;
}
