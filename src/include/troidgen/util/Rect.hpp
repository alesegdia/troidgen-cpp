#pragma once
#include "Vec2.hpp"
#include <string>

namespace troidgen {

struct Rect {
    Vec2 position;
    Vec2 size;

    Rect(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f);
    Rect(const Rect& r);

    float x1() const;
    float y1() const;
    float x2() const;
    float y2() const;

    bool collideWith(const Rect& other) const;
    bool isTouching(const Rect& other) const;

    std::string toString() const;
};

} // namespace troidgen
