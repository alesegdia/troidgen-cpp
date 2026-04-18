#include "troidgen/LayoutBuilder.hpp"
#include "troidgen/LayoutBuilderConfig.hpp"
#include "troidgen/GraphBuilder.hpp"
#include "troidgen/LinkBuilder.hpp"
#include "troidgen/MinSizeRoomGroupValidator.hpp"
#include "troidgen/RandomRoomProvider.hpp"
#include "troidgen/OverlapSolverConfig.hpp"
#include "troidgen/renderer/RectDebugger.hpp"
#include "troidgen/util/Rect.hpp"
#include <iostream>
#include <memory>

using namespace troidgen;

int main() {
    LayoutBuilder lb;
    LayoutBuilderConfig lbc;
    lbc.numIterations = 20;

    OverlapSolverConfig osc;
    osc.separationParameter = 1.0f;
    osc.enableTweakNearSeparation = false;
    osc.resolution = 64;
    osc.enclosingRect = Rect(-20, -10, 40, 20);
    lbc.osc = osc;
    lbc.spawnRect = Rect(-20, -8, 40, 16);

    RandomRoomProvider roomProvider(1, 4);
    MinSizeRoomGroupValidator msrge(10);
    auto result = lb.generate(lbc, roomProvider, msrge);

    GraphBuilder gb;
    gb.build(result);

    for (const auto& r : result) std::cout << r->toString() << "\n";

    LinkBuilder linksb;
    linksb.generate(result);

    RectDebugger rd(result, 800, 600, &osc.enclosingRect);
    rd.Show();

    std::cout << "Test_LayoutBuilder2 done, rooms: " << result.size() << "\n";
    return 0;
}
