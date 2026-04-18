#pragma once
#include "Direction.hpp"
#include "troidgen/util/Vec2.hpp"

namespace troidgen {

// Forward declaration to break circular dependency
struct Room;

struct Link {
    Vec2 relCoord;
    Direction direction;
    Room* ownerRoom = nullptr;
    Room* connectedRoom = nullptr;
    Link* connectedLink = nullptr;

    Link(float x, float y, Direction dir, Room* owner);
    // Copy-link constructor: copies relCoord/direction, sets new owner, no connections
    Link(const Link& link, Room* owner);

    Vec2 getAbsCoords() const;

    void disconnect();
    Direction getOppositeDirection() const;
    void connectTo(Link* other);
    bool isConnected() const;
    bool canConnect(const Link& l) const;
};

} // namespace troidgen
