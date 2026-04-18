#include "troidgen/FloydWarshallSolver.hpp"
#include <limits>
#include <algorithm>

namespace troidgen {

void FloydWarshallSolver::Solve(UpperMatrix2D<float>& matrix) {
    min_distances = &matrix;

    UpperMatrix2D<int> next(matrix.cols, matrix.rows, -1);

    std::vector<int> used;
    bool ok = false;
    for (int i = 0; i < matrix.cols; i++) {
        ok = false;
        for (int j = 0; j < matrix.cols; j++) {
            if (i != j && matrix.GetUpper(i, j) != std::numeric_limits<float>::max()) {
                if (!ok) used.push_back(i);
                next.Set(i, j, j);
                ok = true;
            }
        }
    }

    for (int i = 0; i < matrix.cols; i++) {
        min_distances->Set(i, i, 0.0f);
    }

    std::sort(used.begin(), used.end());

    int usedSz = static_cast<int>(used.size());
    for (int ki = 0; ki < usedSz; ki++) { int k = used[static_cast<size_t>(ki)];
        for (int ii = 0; ii < usedSz; ii++) { int i = used[static_cast<size_t>(ii)];
            for (int ji = ii; ji < usedSz; ji++) { int j = used[static_cast<size_t>(ji)];
                float dij = min_distances->GetUpper(i, j);
                float dik = min_distances->GetUpper(i, k);
                float dkj = min_distances->GetUpper(k, j);
                if (dij > dik + dkj) {
                    float newvalue = dik + dkj;
                    min_distances->SetUpper(i, j, newvalue);
                    next.Set(i, j, next.Get(i, k));
                    next.Set(j, i, next.Get(j, k));
                }
            }
        }
    }

    int biggest_r1 = 0, biggest_r2 = 0;
    float biggest_value = std::numeric_limits<float>::min();
    for (int i = 0; i < matrix.cols; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            float val = min_distances->GetUpper(i, j);
            if (val > 0.1f && val < 100000000.0f && val > biggest_value) {
                biggest_r1 = i;
                biggest_r2 = j;
                biggest_value = val;
            }
        }
    }
    spawn_room = biggest_r1;
    goal_room  = biggest_r2;
    distance   = biggest_value;

    int u = spawn_room;
    path.clear();
    path.push_back(u);
    int v = goal_room;
    while (u != v) {
        u = next.Get(u, v);
        path.push_back(u);
    }
}

} // namespace troidgen
