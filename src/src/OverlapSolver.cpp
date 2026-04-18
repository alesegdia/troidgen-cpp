#include "troidgen/OverlapSolver.hpp"
#include "troidgen/util/RNG.hpp"
#include <algorithm>
#include <cmath>

namespace troidgen {

std::vector<std::shared_ptr<Room>> OverlapSolver::solve(
    const OverlapSolverConfig& osc,
    const std::vector<std::shared_ptr<Room>>& testRects) {
    static const std::vector<std::shared_ptr<Room>> empty;
    return solve(osc, testRects, empty);
}

std::vector<std::shared_ptr<Room>> OverlapSolver::solve(
    const OverlapSolverConfig& osc,
    const std::vector<std::shared_ptr<Room>>& testRects,
    const std::vector<std::shared_ptr<Room>>& outputLayout) {

    // Create scaled copies of testRects; add outputLayout rooms as-is
    std::vector<std::shared_ptr<Room>> rects;
    for (const auto& r : testRects) {
        auto rr = std::make_shared<Room>(*r);
        rr->position.x *= static_cast<float>(osc.resolution);
        rr->position.y *= static_cast<float>(osc.resolution);
        rr->size.x     *= static_cast<float>(osc.resolution);
        rr->size.y     *= static_cast<float>(osc.resolution);
        rects.push_back(std::move(rr));
    }
    rects.insert(rects.end(), outputLayout.begin(), outputLayout.end());

    bool overlap = true;
    while (overlap) {
        overlap = false;
        for (const auto& r1 : rects) {
            // Skip rooms from outputLayout (they're fixed anchors)
            bool isFixed = std::find(outputLayout.begin(), outputLayout.end(), r1)
                           != outputLayout.end();
            if (!isFixed) {
                Vec2 push(0.0f, 0.0f);
                bool thisOverlap = false;

                for (const auto& r2 : rects) {
                    if (r1.get() != r2.get() && r1->collideWith(*r2)) {
                        overlap = true;
                        thisOverlap = true;
                        Vec2 normal = r1->centroid().sub(r2->centroid());
                        push.x += normal.x;
                        push.y += normal.y;
                    }
                }

                if (thisOverlap) {
                    float sepPar = osc.separationParameter;
                    if (osc.enableTweakNearSeparation) {
                        sepPar = osc.separationParameter * (r1->size.x + r1->size.y);
                    }

                    if (push.x != 0.0f || push.y != 0.0f) {
                        if (osc.enableRandomDisplacement) {
                            push.x += RNG::rng->nextFloat() / 5.0f;
                            push.y += RNG::rng->nextFloat() / 5.0f;
                        }
                        repulse(*r1, push, sepPar);
                    } else if (osc.enableRandomPushIfZero) {
                        Vec2 rndPush(nonZeroRnd(-1, 1), nonZeroRnd(-1, 1));
                        repulse(*r1, rndPush, sepPar);
                    }

                    if (r1->position.x < osc.enclosingLeft())
                        r1->position.x = osc.enclosingLeft();
                    if (r1->position.x + r1->size.x > osc.enclosingRight())
                        r1->position.x = osc.enclosingRight();
                    if (r1->position.y < osc.enclosingTop())
                        r1->position.y = osc.enclosingTop();
                    if (r1->position.y + r1->size.y > osc.enclosingBottom())
                        r1->position.y = osc.enclosingBottom();

                    if (osc.earlyBreak) break;
                }
            }
        }
    }

    // Scale back all rooms
    for (auto& r : rects) {
        r->position.x = std::round(r->position.x / static_cast<float>(osc.resolution));
        r->position.y = std::round(r->position.y / static_cast<float>(osc.resolution));
        r->size.x /= static_cast<float>(osc.resolution);
        r->size.y /= static_cast<float>(osc.resolution);
    }

    return rects;
}

float OverlapSolver::nonZeroRnd(int min, int max) {
    int rnd = 0;
    while (rnd == 0) {
        rnd = RNG::rng->nextInt(min, max);
    }
    return static_cast<float>(rnd);
}

void OverlapSolver::repulse(Rect& r1, Vec2& push, float separationParameter) {
    push.nor();
    push.scale(separationParameter / std::max(push.mod(), 1.0f));

    if (std::abs(push.x) < 1.0f) {
        push.x = (push.x >= 0.0f) ? 1.0f : -1.0f;
    }
    if (std::abs(push.y) < 1.0f) {
        push.y = (push.y >= 0.0f) ? 1.0f : -1.0f;
    }

    r1.position.x += push.x;
    r1.position.y += push.y;
}

} // namespace troidgen
