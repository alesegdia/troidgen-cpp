#pragma once
#include <vector>
#include <string>
#include <initializer_list>
#include <cassert>

namespace troidgen {

class RestrictionSet {
public:
    RestrictionSet();
    explicit RestrictionSet(int size);
    RestrictionSet(int size, std::initializer_list<bool> constraints);

    bool has(int index) const;
    void set(int index, bool value);

    // Returns true if every bit set in 'this' is also set in 'target'
    bool resolves(const RestrictionSet& target) const;

    bool operator==(const RestrictionSet& other) const;

    std::string toString() const;

    static RestrictionSet FromString(const std::string& s);

    int size() const { return static_cast<int>(restrictions.size()); }

private:
    std::vector<bool> restrictions;
};

} // namespace troidgen
