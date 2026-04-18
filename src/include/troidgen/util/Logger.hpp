#pragma once

namespace troidgen {

class Logger {
public:
    template<typename T>
    static void Log(const T&) {}
};

} // namespace troidgen
