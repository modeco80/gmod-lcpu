#include "OsLibrary.hpp"

#if defined(_WIN32)
#include "OsLibrary.win32.cpp"
#elif defined(__linux__)
#include "OsLibrary.linux.cpp"
#else
#error No OsLibrary implementation for this platform
#endif
