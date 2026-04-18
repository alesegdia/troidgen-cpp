#pragma once
#include "IRoomGroupValidator.hpp"

namespace troidgen {

class MinSpaceRoomGroupValidator : public IRoomGroupValidator {
public:
    explicit MinSpaceRoomGroupValidator(float minSpace) : min(minSpace) {}

    bool validate(const std::vector<std::shared_ptr<Room>>& outputLayout) override {
        float totalArea = 0.0f;
        for (const auto& r : outputLayout) {
            totalArea += r->size.x * r->size.y;
        }
        return totalArea > min;
    }

private:
    float min;
};

} // namespace troidgen
