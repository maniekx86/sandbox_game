#ifndef SYSCROSS_HPP
#define SYSCROSS_HPP

#include <string>
#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__) || defined(__MACH__)

#else
#include <cstdlib>
#endif

namespace syscross {
    bool openURL(const std::string& url);
};


#endif // SYSCROSS_HPP
