#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace troidgen {

template <typename T>
class Matrix2D {
public:
    std::vector<T> data;
    int cols;
    int rows;

    Matrix2D(int rows, int cols, T def)
        : rows(rows), cols(cols), data(static_cast<size_t>(rows * cols), def) {}

    Matrix2D(const Matrix2D<T>& other)
        : rows(other.rows), cols(other.cols), data(other.data) {}

    Matrix2D() : rows(0), cols(0) {}

    T Get(int row, int col) const {
        if (row < 0) row = 0;
        if (row >= rows) row = rows - 1;
        if (col < 0) col = 0;
        if (col >= cols) col = cols - 1;
        return data[static_cast<size_t>(GetPos(row, col))];
    }

    int GetPos(int row, int col) const {
        return row * cols + col;
    }

    void Set(int row, int col, T val) {
        if (row < 0) row = 0;
        if (row >= rows) row = rows - 1;
        if (col < 0) col = 0;
        if (col >= cols) col = cols - 1;
        data[static_cast<size_t>(GetPos(row, col))] = val;
    }

    void Fill(T val) {
        for (auto& elem : data) {
            elem = val;
        }
    }

    void CreateFromData(const std::vector<T>& d, int r, int c) {
        data = d;
        rows = r;
        cols = c;
    }

    std::string toString() const {
        std::ostringstream oss;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                oss << "\t" << Get(i, j);
            }
            oss << "\n";
        }
        return oss.str();
    }
};

} // namespace troidgen
