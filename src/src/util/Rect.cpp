#include "troidgen/util/Rect.hpp"
#include <sstream>

namespace troidgen {

Rect::Rect(float x, float y, float w, float h)
    : position(x, y), size(w, h) {}

Rect::Rect(const Rect& r)
    : position(r.position), size(r.size) {}

float Rect::x1() const { return position.x; }
float Rect::y1() const { return position.y; }
float Rect::x2() const { return position.x + size.x; }
float Rect::y2() const { return position.y + size.y; }

bool Rect::collideWith(const Rect& other) const {
    return x1() < other.x2() && x2() > other.x1() &&
           y1() < other.y2() && y2() > other.y1();
}

bool Rect::isTouching(const Rect& other) const {
    bool tl = (x1() == other.x2() && y1() == other.y2());
    bool br = (x2() == other.x1() && y2() == other.y1());
    bool bl = (x1() == other.x2() && y2() == other.y1());
    bool tr = (x2() == other.x1() && y1() == other.y2());

    return !(other.x1() - x2() > 0.0f ||
             x1() - other.x2() > 0.0f ||
             other.y1() - y2() > 0.0f ||
             y1() - other.y2() > 0.0f) &&
           !tl && !br && !bl && !tr;
}

std::string Rect::toString() const {
    std::ostringstream oss;
    oss << "[" << position.x << "," << position.y
        << " - " << size.x << "," << size.y << "]";
    return oss.str();
}

} // namespace troidgen
