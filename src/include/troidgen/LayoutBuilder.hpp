#pragma once
#include "LayoutBuilderConfig.hpp"
#include "IRoomProvider.hpp"
#include "IRoomGroupValidator.hpp"
#include "troidgen/room/Room.hpp"
#include <vector>
#include <memory>

namespace troidgen {

class LayoutBuilder {
public:
    std::vector<std::shared_ptr<Room>> generate(
        LayoutBuilderConfig& lbc,
        IRoomProvider& geometryProvider,
        IRoomGroupValidator& rge);

    std::vector<std::shared_ptr<Room>> generate(
        LayoutBuilderConfig& lbc,
        IRoomProvider& geometryProvider,
        IRoomGroupValidator& rge,
        const std::vector<std::shared_ptr<Room>>& fixedGroup);

private:
    static std::vector<std::shared_ptr<Room>> substraction(
        const std::vector<std::shared_ptr<Room>>& a,
        const std::vector<std::shared_ptr<Room>>& b);

    static std::vector<std::shared_ptr<Room>> getBestGroup(
        const std::vector<std::vector<std::shared_ptr<Room>>>& groups);
};

} // namespace troidgen
