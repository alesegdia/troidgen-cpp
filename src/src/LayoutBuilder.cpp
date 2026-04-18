#include "troidgen/LayoutBuilder.hpp"
#include "troidgen/OverlapSolver.hpp"
#include "troidgen/GroupExtractor.hpp"
#include "troidgen/LinkBuilder.hpp"
#include "troidgen/renderer/RectDebugger.hpp"
#include "troidgen/util/RNG.hpp"
#include "troidgen/util/Logger.hpp"
#include "troidgen/util/MiscRoomUtils.hpp"
#include <algorithm>

namespace troidgen {

std::vector<std::shared_ptr<Room>> LayoutBuilder::generate(
    LayoutBuilderConfig& lbc,
    IRoomProvider& geometryProvider,
    IRoomGroupValidator& rge) {
    static const std::vector<std::shared_ptr<Room>> empty;
    return generate(lbc, geometryProvider, rge, empty);
}

std::vector<std::shared_ptr<Room>> LayoutBuilder::generate(
    LayoutBuilderConfig& lbc,
    IRoomProvider& geometryProvider,
    IRoomGroupValidator& rge,
    const std::vector<std::shared_ptr<Room>>& fixedGroup) {

    RNG::rng = std::make_unique<RNG>();
    OverlapSolver os;

    std::vector<std::shared_ptr<Room>> outputLayout = fixedGroup;

    std::vector<std::shared_ptr<Room>> checkGroup = substraction(outputLayout, fixedGroup);

    while (!rge.validate(checkGroup)) {
        Logger::Log("in");
        auto testRects = geometryProvider.provideList(lbc.numRooms);
        MiscRoomUtils::RandomPlaceInRange(testRects, lbc.spawnRect);

        testRects = os.solve(lbc.osc, testRects, outputLayout);
        Logger::Log("out");

        GroupExtractor ge;
        auto groups = ge.solve(testRects);
        for (auto& lr : groups) {
            LinkBuilder lb;
            lb.generate(lr);
        }
        outputLayout = getBestGroup(groups);
        geometryProvider.notifySelected(outputLayout);

        Logger::Log(static_cast<int>(outputLayout.size()));

        if (lbc.interactive) {
            auto checkGroupDebug = substraction(outputLayout, fixedGroup);
            RectDebugger(checkGroupDebug, 800, 600).Show();
        }

        checkGroup = substraction(outputLayout, fixedGroup);

        Logger::Log(static_cast<int>(checkGroup.size()));
    }

    return outputLayout;
}

std::vector<std::shared_ptr<Room>> LayoutBuilder::substraction(
    const std::vector<std::shared_ptr<Room>>& a,
    const std::vector<std::shared_ptr<Room>>& b) {
    std::vector<std::shared_ptr<Room>> result;
    for (const auto& r : a) {
        if (std::find(b.begin(), b.end(), r) == b.end()) {
            result.push_back(r);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Room>> LayoutBuilder::getBestGroup(
    const std::vector<std::vector<std::shared_ptr<Room>>>& groups) {
    const std::vector<std::shared_ptr<Room>>* best = &groups[0];
    for (const auto& group : groups) {
        if (group.size() > best->size()) {
            best = &group;
        }
    }
    return *best;
}

} // namespace troidgen
