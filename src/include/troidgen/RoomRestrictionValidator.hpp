#pragma once
#include "IRoomValidator.hpp"
#include "troidgen/restriction/RestrictionSet.hpp"

namespace troidgen {

class RoomRestrictionValidator : public IRoomValidator {
public:
    explicit RoomRestrictionValidator(const RestrictionSet& rs) : rs(rs) {}

    bool validate(Room* r) override {
        if (!r->rinfo) return false;
        return r->rinfo->restriction == rs;
    }

private:
    RestrictionSet rs;
};

} // namespace troidgen
