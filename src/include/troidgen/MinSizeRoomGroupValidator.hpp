#pragma once
#include "IRoomGroupValidator.hpp"

namespace troidgen {

class MinSizeRoomGroupValidator : public IRoomGroupValidator {
public:
    explicit MinSizeRoomGroupValidator(int minSize) : min(minSize) {}

    bool validate(const std::vector<std::shared_ptr<Room>>& outputLayout) override {
        return static_cast<int>(outputLayout.size()) > min;
    }

private:
    int min;
};

} // namespace troidgen
