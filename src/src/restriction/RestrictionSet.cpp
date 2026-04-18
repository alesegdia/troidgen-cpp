#include "troidgen/restriction/RestrictionSet.hpp"
#include <cassert>
#include <stdexcept>

namespace troidgen {

RestrictionSet::RestrictionSet() {}

RestrictionSet::RestrictionSet(int sz) : restrictions(static_cast<size_t>(sz), false) {}

RestrictionSet::RestrictionSet(int sz, std::initializer_list<bool> constraints)
    : restrictions(static_cast<size_t>(sz), false) {
    assert(sz == static_cast<int>(constraints.size()));
    int i = 0;
    for (bool b : constraints) {
        restrictions[static_cast<size_t>(i++)] = b;
    }
}

bool RestrictionSet::has(int index) const {
    assert(index >= 0 && index < static_cast<int>(restrictions.size()));
    return restrictions[static_cast<size_t>(index)];
}

void RestrictionSet::set(int index, bool value) {
    restrictions[static_cast<size_t>(index)] = value;
}

bool RestrictionSet::resolves(const RestrictionSet& target) const {
    for (int i = 0; i < static_cast<int>(restrictions.size()); i++) {
        if (has(i) && !target.has(i)) {
            return false;
        }
    }
    return true;
}

bool RestrictionSet::operator==(const RestrictionSet& other) const {
    if (restrictions.size() != other.restrictions.size()) return false;
    for (size_t i = 0; i < restrictions.size(); i++) {
        if (restrictions[i] != other.restrictions[i]) return false;
    }
    return true;
}

std::string RestrictionSet::toString() const {
    std::string s;
    for (size_t i = 0; i < restrictions.size(); i++) {
        s += (restrictions[i] ? '1' : '0');
    }
    return s;
}

RestrictionSet RestrictionSet::FromString(const std::string& s) {
    RestrictionSet rs(static_cast<int>(s.size()));
    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        char bit = s[static_cast<size_t>(i)];
        if (bit == '1') {
            rs.set(i, true);
        } else if (bit == '0') {
            rs.set(i, false);
        } else {
            assert(false);
        }
    }
    return rs;
}

} // namespace troidgen
