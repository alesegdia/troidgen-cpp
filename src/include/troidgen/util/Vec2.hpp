#pragma once
#include <string>
#include <cmath>

namespace troidgen {

struct Vec2 {
    float x;
    float y;

    Vec2(float x = 0.0f, float y = 0.0f);
    Vec2(const Vec2& o);

    void Set(float f, float g);
    void set(const Vec2& other);

    Vec2 Add(const Vec2& other) const;
    Vec2 sub(const Vec2& other) const;
    float distance(const Vec2& other) const;
    float mod() const;
    void nor();
    void scale(float f);

    bool operator==(const Vec2& other) const;
    Vec2 clone() const;

    std::string toString() const;
};

} // namespace troidgen
