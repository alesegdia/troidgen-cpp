#include "troidgen/room/Link.hpp"
#include "troidgen/room/Room.hpp"
#include <cassert>
#include <cmath>

namespace troidgen {

Link::Link(float x, float y, Direction dir, Room* owner)
    : relCoord(x, y), direction(dir), ownerRoom(owner) {}

Link::Link(const Link& link, Room* owner)
    : relCoord(link.relCoord), direction(link.direction), ownerRoom(owner) {}

Vec2 Link::getAbsCoords() const {
    return Vec2(ownerRoom->position.x + relCoord.x,
                ownerRoom->position.y + relCoord.y);
}

void Link::disconnect() {
    connectedLink->connectedRoom = nullptr;
    connectedLink->connectedLink = nullptr;
    connectedRoom = nullptr;
    connectedLink = nullptr;
}

Direction Link::getOppositeDirection() const {
    switch (direction) {
        case Direction::TOP:   return Direction::DOWN;
        case Direction::DOWN:  return Direction::TOP;
        case Direction::LEFT:  return Direction::RIGHT;
        case Direction::RIGHT: return Direction::LEFT;
        default:               return Direction::NODIR;
    }
}

void Link::connectTo(Link* other) {
    this->connectedRoom = other->ownerRoom;
    this->connectedLink = other;
    other->connectedRoom = this->ownerRoom;
    other->connectedLink = this;
}

bool Link::isConnected() const {
    return connectedRoom != nullptr && connectedLink != nullptr;
}

bool Link::canConnect(const Link& l) const {
    assert(l.direction != Direction::NODIR);
    if (l.ownerRoom != ownerRoom &&
        l.ownerRoom->isTouching(*ownerRoom) &&
        getOppositeDirection() == l.direction) {

        Vec2 myAbs = getAbsCoords();
        Vec2 otherAbs = l.getAbsCoords();

        float dx = myAbs.x - otherAbs.x;
        float dy = myAbs.y - otherAbs.y;

        switch (direction) {
            case Direction::TOP:   return dx == 0.0f && dy == -1.0f;
            case Direction::DOWN:  return dx == 0.0f && dy ==  1.0f;
            case Direction::LEFT:  return dx == 1.0f && dy ==  0.0f;
            case Direction::RIGHT: return dx == -1.0f && dy == 0.0f;
            default: return false;
        }
    }
    return false;
}

} // namespace troidgen
