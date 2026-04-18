#pragma once

namespace troidgen {

struct Link;
struct Room;

struct LinkPair {
    Link* lA;
    Link* lB;

    LinkPair(Link* a, Link* b) : lA(a), lB(b) {}

    void connect();
};

} // namespace troidgen
