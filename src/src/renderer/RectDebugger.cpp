#include "troidgen/renderer/RectDebugger.hpp"
#include "troidgen/room/Link.hpp"
#include "troidgen/room/Room.hpp"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cfloat>
#include <cmath>

namespace troidgen {

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------

RectDebugger::RectDebugger(std::vector<std::shared_ptr<Room>> rooms, int width, int height)
    : rooms_(std::move(rooms)), winWidth_(width), winHeight_(height), enclosing_(nullptr) {}

RectDebugger::RectDebugger(std::vector<std::shared_ptr<Room>> rooms, int width, int height, Rect* enclosing)
    : rooms_(std::move(rooms)), winWidth_(width), winHeight_(height), enclosing_(enclosing) {}

// ---------------------------------------------------------------------------
// computeScale
// ---------------------------------------------------------------------------

float RectDebugger::computeScale(float& outOffsetX, float& outOffsetY) const {
    if (rooms_.empty()) {
        outOffsetX = static_cast<float>(winWidth_)  / 2.0f;
        outOffsetY = static_cast<float>(winHeight_) / 2.0f;
        return 1.0f;
    }

    float minX =  FLT_MAX, minY =  FLT_MAX;
    float maxX = -FLT_MAX, maxY = -FLT_MAX;

    for (const auto& r : rooms_) {
        minX = std::min(minX, r->position.x);
        minY = std::min(minY, r->position.y);
        maxX = std::max(maxX, r->position.x + r->size.x);
        maxY = std::max(maxY, r->position.y + r->size.y);
    }

    float spanX = maxX - minX;
    float spanY = maxY - minY;
    if (spanX <= 0.0f) spanX = 1.0f;
    if (spanY <= 0.0f) spanY = 1.0f;

    const float pad = 20.0f;
    float scaleX = (static_cast<float>(winWidth_)  - pad) / spanX;
    float scaleY = (static_cast<float>(winHeight_) - pad) / spanY;
    float scale  = std::min(scaleX, scaleY);

    // Centre the layout in the window
    float layoutPixW = spanX * scale;
    float layoutPixH = spanY * scale;
    outOffsetX = (static_cast<float>(winWidth_)  - layoutPixW) / 2.0f - minX * scale;
    outOffsetY = (static_cast<float>(winHeight_) - layoutPixH) / 2.0f - minY * scale;

    return scale;
}

// ---------------------------------------------------------------------------
// renderGrid
// ---------------------------------------------------------------------------

void RectDebugger::renderGrid(SDL_Renderer* renderer, float scale, float ox, float oy) const {
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 80);

    // Determine world-space extents visible in the window
    float worldLeft   = (0.0f       - ox) / scale;
    float worldRight  = (static_cast<float>(winWidth_)  - ox) / scale;
    float worldTop    = (0.0f       - oy) / scale;
    float worldBottom = (static_cast<float>(winHeight_) - oy) / scale;

    // Clamp grid step so lines are at least 5 px apart
    float step = 1.0f;
    while (step * scale < 5.0f) step *= 2.0f;

    // Vertical lines
    float startX = std::floor(worldLeft  / step) * step;
    for (float wx = startX; wx <= worldRight; wx += step) {
        int px = static_cast<int>(std::round(wx * scale + ox));
        SDL_RenderDrawLine(renderer, px, 0, px, winHeight_);
    }

    // Horizontal lines
    float startY = std::floor(worldTop / step) * step;
    for (float wy = startY; wy <= worldBottom; wy += step) {
        int py = static_cast<int>(std::round(wy * scale + oy));
        SDL_RenderDrawLine(renderer, 0, py, winWidth_, py);
    }
}

// ---------------------------------------------------------------------------
// renderRects
// ---------------------------------------------------------------------------

void RectDebugger::renderRects(SDL_Renderer* renderer, float scale, float ox, float oy) const {
    for (const auto& room : rooms_) {
        int px = static_cast<int>(std::round(room->position.x * scale + ox));
        int py = static_cast<int>(std::round(room->position.y * scale + oy));
        int pw = static_cast<int>(std::round(room->size.x * scale));
        int ph = static_cast<int>(std::round(room->size.y * scale));

        // Blue filled rect (room body)
        SDL_Rect body{px, py, std::max(pw, 1), std::max(ph, 1)};
        SDL_SetRenderDrawColor(renderer, 0, 0, 180, 255);
        SDL_RenderFillRect(renderer, &body);

        // Magenta inset (+1 px on each side)
        if (pw > 2 && ph > 2) {
            SDL_Rect inset{px + 1, py + 1, pw - 2, ph - 2};
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            SDL_RenderFillRect(renderer, &inset);
        }

        // Link squares (3x3 px each)
        for (const auto& link : room->links) {
            // Compute absolute link position
            float absX = room->position.x + link.relCoord.x;
            float absY = room->position.y + link.relCoord.y;

            // Offset by direction so the square sits on the wall face
            if (link.direction == Direction::RIGHT) absX += 1.0f;
            if (link.direction == Direction::DOWN)  absY += 1.0f;

            int lx = static_cast<int>(std::round(absX * scale + ox)) - 1;
            int ly = static_cast<int>(std::round(absY * scale + oy)) - 1;

            SDL_Rect sq{lx, ly, 3, 3};
            if (link.connectedRoom != nullptr) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);   // green = connected
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);   // dark-blue = free
            }
            SDL_RenderFillRect(renderer, &sq);
        }
    }

    // Optional enclosing rect outline in light-blue
    if (enclosing_ != nullptr) {
        int ex = static_cast<int>(std::round(enclosing_->position.x * scale + ox));
        int ey = static_cast<int>(std::round(enclosing_->position.y * scale + oy));
        int ew = static_cast<int>(std::round(enclosing_->size.x * scale));
        int eh = static_cast<int>(std::round(enclosing_->size.y * scale));
        SDL_Rect outline{ex, ey, std::max(ew, 1), std::max(eh, 1)};
        SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
        SDL_RenderDrawRect(renderer, &outline);
    }
}

// ---------------------------------------------------------------------------
// Show
// ---------------------------------------------------------------------------

void RectDebugger::Show() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return;

    SDL_Window* window = SDL_CreateWindow(
        "RectDebugger",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        winWidth_, winHeight_,
        SDL_WINDOW_SHOWN);
    if (!window) { SDL_Quit(); return; }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) { SDL_DestroyWindow(window); SDL_Quit(); return; }

    // Render once
    float ox, oy;
    float scale = computeScale(ox, oy);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderGrid(renderer, scale, ox, oy);
    renderRects(renderer, scale, ox, oy);
    SDL_RenderPresent(renderer);

    // Block until Enter or window close
    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN ||
                    event.key.keysym.sym == SDLK_KP_ENTER) {
                    running = false;
                }
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

} // namespace troidgen
