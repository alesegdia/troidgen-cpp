#pragma once
#include "IRoomGroupValidator.hpp"
#include "ExactRoomProvider.hpp"
#include "troidgen/util/Logger.hpp"

namespace troidgen {

class ExactRoomProviderValidator : public IRoomGroupValidator {
public:
    explicit ExactRoomProviderValidator(ExactRoomProvider* rp) : rp(rp) {}

    bool validate(const std::vector<std::shared_ptr<Room>>& group) override {
        Logger::Log(static_cast<int>(group.size()));
        return rp->isOk();
    }

private:
    ExactRoomProvider* rp;
};

} // namespace troidgen
