#pragma once
#include "troidgen/util/Rect.hpp"
#include "troidgen/restriction/RestrictionSet.hpp"
#include "Link.hpp"
#include "LinkPair.hpp"
#include "RoomInfo.hpp"
#include "RoomType.hpp"
#include <vector>
#include <string>
#include <optional>

namespace troidgen {

struct Room : public Rect {
    std::vector<Link> links;
    std::optional<RestrictionSet> restrictionSet;
    std::vector<Room*> neighboors;
    RoomType rtype = RoomType::COMMON;
    RoomInfo* rinfo = nullptr;
    bool isVisited = false;
    bool opened = true;

    // Construct from RoomInfo template
    explicit Room(RoomInfo* rinfo);

    // Construct with explicit size and optional restriction (computeAllLinks)
    Room(float width, float height, const RestrictionSet* rs = nullptr);

    // Copy constructor: copies links with 'this' as owner, no connections
    Room(const Room& r);

    // Construct with explicit position and size (computeAllLinks)
    Room(float x, float y, float w, float h);

    std::vector<LinkPair> getPossibleConnections(Room* other);

    std::vector<Link*> linksFacing(Direction dir);
    bool hasLinkFacing(Direction dir) const;

    bool directlyConnectedWith(Room* r2) const;

    bool connectsWith(Room* other);

    Vec2 centroid() const;

    std::string toString() const;

private:
    void computeAllLinks();
    bool connectsWith_(Room* other, std::vector<Room*>& visited);
};

} // namespace troidgen
