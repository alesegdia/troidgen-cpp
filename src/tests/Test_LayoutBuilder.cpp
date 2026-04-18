#include "troidgen/LayoutBuilder.hpp"
#include "troidgen/LayoutBuilderConfig.hpp"
#include "troidgen/GraphBuilder.hpp"
#include "troidgen/LinkBuilder.hpp"
#include "troidgen/MinSizeRoomGroupValidator.hpp"
#include "troidgen/RandomRoomProvider.hpp"
#include "troidgen/OverlapSolverConfig.hpp"
#include "troidgen/util/Rect.hpp"
#include <iostream>
#include <memory>

using namespace troidgen;

int main() {
    LayoutBuilder lb;
    LayoutBuilderConfig lbc;
    lbc.spawnRect = Rect(-8, -8, 10, 10);
    lbc.numIterations = 20;

    OverlapSolverConfig osc;
    osc.separationParameter = 1.0f;
    osc.enableTweakNearSeparation = false;
    osc.resolution = 64;
    osc.enclosingRect = Rect(-20, -15, 40, 30);
    lbc.osc = osc;

    RandomRoomProvider roomProvider(1, 4);
    MinSizeRoomGroupValidator msrge(10);
    auto result = lb.generate(lbc, roomProvider, msrge);

    lbc.spawnRect.position.y += 3;
    MinSizeRoomGroupValidator msrge2(20);
    result = lb.generate(lbc, roomProvider, msrge2, result);

    lbc.spawnRect.position.x -= 3;
    MinSizeRoomGroupValidator msrge3(30);
    result = lb.generate(lbc, roomProvider, msrge3, result);

    GraphBuilder gb;
    auto m = gb.build(result);
    std::cout << m.toString();

    for (const auto& r : result) std::cout << r->toString() << "\n";

    LinkBuilder linksb;
    linksb.generate(result);

    std::cout << "Test_LayoutBuilder done, rooms: " << result.size() << "\n";
    return 0;
}
