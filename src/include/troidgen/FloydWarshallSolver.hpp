#pragma once
#include "troidgen/util/UpperMatrix2D.hpp"
#include <vector>

namespace troidgen {

class FloydWarshallSolver {
public:
    void Solve(UpperMatrix2D<float>& matrix);

    int GetSpawnRoom() const { return spawn_room; }
    int GetGoalRoom() const  { return goal_room; }
    float GetDistance() const { return distance; }
    const std::vector<int>& GetPath() const { return path; }

private:
    UpperMatrix2D<float>* min_distances = nullptr;
    int spawn_room = 0;
    int goal_room  = 0;
    float distance = 0.0f;
    std::vector<int> path;
};

} // namespace troidgen
