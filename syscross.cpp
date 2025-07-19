#include "syscross.hpp"

bool syscross::openURL(const std::string& url) {

#if defined(_WIN32) || defined(_WIN64)
    HINSTANCE result = ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if ((reinterpret_cast<intptr_t>(result)) <= 32) {
        return false;
    }
#elif defined(__APPLE__) || defined(__MACH__)
    std::string command = "open " + url;
    if (system(command.c_str()) != 0) {
        return false;
    }
#elif defined(__linux__)
    std::string command = "xdg-open " + url;
    if (system(command.c_str()) != 0) {
        return false;
    }
#else
    // unsupported OS
    return false;
#endif
    return true;
}