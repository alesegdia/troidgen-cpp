#pragma once
#include "Matrix2D.hpp"
#include <iostream>

namespace troidgen {

template <typename T>
class UpperMatrix2D : public Matrix2D<T> {
public:
    UpperMatrix2D(int rows, int cols, T def)
        : Matrix2D<T>(rows, cols, def) {}

    UpperMatrix2D(const UpperMatrix2D<T>& other)
        : Matrix2D<T>(other) {}

    T GetUpper(int row, int col) const {
        if (row < col) {
            return Matrix2D<T>::Get(row, col);
        } else {
            return Matrix2D<T>::Get(col, row);
        }
    }

    void SetUpper(int row, int col, T val) {
        if (row < col) {
            Matrix2D<T>::Set(row, col, val);
        } else {
            Matrix2D<T>::Set(col, row, val);
        }
    }

    void Debug() const {
        for (int i = 0; i < this->cols; i++) {
            for (int j = 0; j < this->rows; j++) {
                std::cout << Matrix2D<T>::Get(i, j) << "\t";
            }
            std::cout << "\n";
        }
    }

    UpperMatrix2D<T> Clone() const {
        return UpperMatrix2D<T>(*this);
    }
};

} // namespace troidgen
