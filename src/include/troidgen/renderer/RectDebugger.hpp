#pragma once
#include "troidgen/room/Room.hpp"
#include "troidgen/util/Rect.hpp"
#include <memory>
#include <vector>

struct SDL_Renderer;

namespace troidgen {

class RectDebugger {
public:
    // Without enclosing rect
    RectDebugger(std::vector<std::shared_ptr<Room>> rooms, int width = 400, int height = 400);
    // With optional enclosing rect (light-blue outline)
    RectDebugger(std::vector<std::shared_ptr<Room>> rooms, int width, int height, Rect* enclosing);

    // Opens an SDL2 window, renders rooms, and blocks until Enter is pressed or window is closed.
    void Show();

private:
    std::vector<std::shared_ptr<Room>> rooms_;
    int   winWidth_;
    int   winHeight_;
    Rect* enclosing_;   // non-owning, nullable

    // Returns pixel scale and writes pixel offsets so the layout is centred in the window.
    float computeScale(float& outOffsetX, float& outOffsetY) const;

    void renderGrid (SDL_Renderer* renderer, float scale, float ox, float oy) const;
    void renderRects(SDL_Renderer* renderer, float scale, float ox, float oy) const;
};

} // namespace troidgen
