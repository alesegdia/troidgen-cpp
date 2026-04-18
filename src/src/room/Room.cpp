#include "troidgen/room/Room.hpp"
#include <algorithm>
#include <sstream>
#include <cassert>

namespace troidgen {

Room::Room(RoomInfo* ri)
    : Rect(0.0f, 0.0f, ri->size.x, ri->size.y),
      restrictionSet(ri->restriction),
      rtype(ri->rtype),
      rinfo(ri) {
    links.reserve(static_cast<size_t>(
        2 * static_cast<int>(ri->size.x) + 2 * static_cast<int>(ri->size.y)));
    for (const auto& linfo : ri->linkInfo) {
        links.emplace_back(linfo.relativeCoordinate.x,
                           linfo.relativeCoordinate.y,
                           linfo.direction, this);
    }
}

Room::Room(float width, float height, const RestrictionSet* rs)
    : Rect(0.0f, 0.0f, width, height) {
    if (rs) restrictionSet = *rs;
    computeAllLinks();
}

Room::Room(const Room& r)
    : Rect(r.position.x, r.position.y, r.size.x, r.size.y),
      restrictionSet(r.restrictionSet),
      rtype(r.rtype),
      rinfo(r.rinfo),
      isVisited(false),
      opened(r.opened) {
    links.reserve(r.links.size());
    for (const auto& link : r.links) {
        links.emplace_back(link, this);
    }
}

Room::Room(float x, float y, float w, float h)
    : Rect(x, y, w, h) {
    computeAllLinks();
}

void Room::computeAllLinks() {
    links.clear();
    int w = static_cast<int>(size.x);
    int h = static_cast<int>(size.y);
    links.reserve(static_cast<size_t>(2 * w + 2 * h));
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            if (y == 0) {
                links.emplace_back(static_cast<float>(x), static_cast<float>(y),
                                   Direction::DOWN, this);
            }
            if (y == h - 1) {
                links.emplace_back(static_cast<float>(x), static_cast<float>(y),
                                   Direction::TOP, this);
            }
            if (x == 0) {
                links.emplace_back(static_cast<float>(x), static_cast<float>(y),
                                   Direction::LEFT, this);
            }
            if (x == w - 1) {
                links.emplace_back(static_cast<float>(x), static_cast<float>(y),
                                   Direction::RIGHT, this);
            }
        }
    }
}

std::vector<LinkPair> Room::getPossibleConnections(Room* other) {
    std::vector<LinkPair> possibleConnections;
    for (auto& l : links) {
        for (auto& ol : other->links) {
            if (l.canConnect(ol)) {
                possibleConnections.emplace_back(&l, &ol);
            }
        }
    }
    return possibleConnections;
}

std::vector<Link*> Room::linksFacing(Direction dir) {
    std::vector<Link*> result;
    for (auto& l : links) {
        if (l.direction == dir) {
            result.push_back(&l);
        }
    }
    return result;
}

bool Room::hasLinkFacing(Direction dir) const {
    for (const auto& l : links) {
        if (l.direction == dir) return true;
    }
    return false;
}

bool Room::directlyConnectedWith(Room* r2) const {
    for (const auto& l : links) {
        if (l.isConnected() && l.connectedRoom == r2) {
            return true;
        }
    }
    return false;
}

bool Room::connectsWith_(Room* other, std::vector<Room*>& visited) {
    for (auto& l : links) {
        if (l.isConnected() && l.connectedRoom == other) {
            return true;
        } else if (std::find(visited.begin(), visited.end(), l.connectedRoom) == visited.end()) {
            visited.push_back(l.connectedRoom);
            if (l.connectedRoom) {
                return l.connectedRoom->connectsWith_(other, visited);
            }
        }
    }
    return false;
}

bool Room::connectsWith(Room* other) {
    std::vector<Room*> visited;
    return connectsWith_(other, visited);
}

Vec2 Room::centroid() const {
    return Vec2(position.x + size.x / 2.0f,
                position.y + size.y / 2.0f);
}

std::string Room::toString() const {
    std::ostringstream oss;
    oss << "(" << position.x << ", " << position.y
        << ", " << size.x << ", " << size.y << ")";
    return oss.str();
}

} // namespace troidgen
