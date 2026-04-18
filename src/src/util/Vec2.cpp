#include "troidgen/util/Vec2.hpp"
#include <cmath>
#include <cassert>
#include <sstream>

namespace troidgen {

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::Vec2(const Vec2& o) : x(o.x), y(o.y) {}

void Vec2::Set(float f, float g) {
    x = f;
    y = g;
}

void Vec2::set(const Vec2& other) {
    x = other.x;
    y = other.y;
}

Vec2 Vec2::Add(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::sub(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

float Vec2::distance(const Vec2& other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Vec2::mod() const {
    return std::sqrt(x * x + y * y);
}

void Vec2::nor() {
    float m = mod();
    assert(m != 0.0f);
    // Java divides by mod() twice (matches original)
    x /= mod();
    y /= mod();
}

void Vec2::scale(float f) {
    x *= f;
    y *= f;
}

bool Vec2::operator==(const Vec2& other) const {
    return x == other.x && y == other.y;
}

Vec2 Vec2::clone() const {
    return Vec2(x, y);
}

std::string Vec2::toString() const {
    std::ostringstream oss;
    oss << "(" << x << "," << y << ")";
    return oss.str();
}

} // namespace troidgen
