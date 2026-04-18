#include "troidgen/ManualRoomProvider.hpp"
#include "troidgen/restriction/RestrictionSet.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/room/Room.hpp"
#include <iostream>
#include <memory>

using namespace troidgen;

static void test(ManualRoomProvider& mgp, int numRooms) {
    std::cout << "\n";
    auto l = mgp.provideList(numRooms);
    for (const auto& r : l) std::cout << r->toString() << "\n";
}

int main() {
    RNG::rng = std::make_unique<RNG>();

    RestrictionSet rs0(4, {true, false, false, false});
    RestrictionSet rs1(4, {true, true,  false, false});
    RestrictionSet rs2(4, {true, true,  false, true});
    RestrictionSet rs3(4, {true, false, true,  false});
    RestrictionSet rs4(4, {true, false, false, true});

    ManualRoomProvider mgp;
    mgp.add(std::make_shared<Room>(3.0f, 3.0f, &rs0));
    mgp.add(std::make_shared<Room>(1.0f, 2.0f, &rs1));
    mgp.add(std::make_shared<Room>(2.0f, 1.0f, &rs2));
    mgp.add(std::make_shared<Room>(1.0f, 3.0f, &rs2));
    mgp.add(std::make_shared<Room>(3.0f, 3.0f, &rs3));
    mgp.add(std::make_shared<Room>(1.0f, 3.0f, &rs3));
    mgp.add(std::make_shared<Room>(2.0f, 1.0f, &rs0));
    mgp.add(std::make_shared<Room>(3.0f, 3.0f, &rs4));
    mgp.add(std::make_shared<Room>(3.0f, 3.0f, &rs0));
    mgp.add(std::make_shared<Room>(3.0f, 3.0f, &rs0));

    test(mgp, 10);
    test(mgp, 10);
    test(mgp, 10);
    test(mgp, 10);
    return 0;
}
