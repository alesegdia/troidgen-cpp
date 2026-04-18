#include "troidgen/util/Rect.hpp"
#include <cassert>
#include <iostream>

using namespace troidgen;

int main() {
    Rect r1(0, 0, 2, 2);
    Rect r2(-1, -1, 2, 2);
    Rect r3(3, 3, 2, 2);
    Rect r4(2, 2, 2, 2);

    assert(r1.collideWith(r2));
    assert(!r1.collideWith(r3));
    assert(!r1.collideWith(r4));

    assert(r2.collideWith(r1));
    assert(!r2.collideWith(r3));
    assert(!r2.collideWith(r4));

    assert(!r3.collideWith(r1));
    assert(!r3.collideWith(r2));
    assert(r3.collideWith(r4));

    assert(!r4.collideWith(r1));
    assert(!r4.collideWith(r2));
    assert(r4.collideWith(r3));

    std::cout << "Test_RectCollision: all assertions passed\n";
    return 0;
}
