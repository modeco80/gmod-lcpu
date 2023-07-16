//! Lucore Assert Wrappers

#pragma once

#include <cstdio>

namespace lucore {

	// TODO: wrapper which uses source_location (we don't need no macros anymore!) 
	[[noreturn]] void ExitMsg(const char* fileName, int fileLine, const char* message);

}

#ifndef NDEBUG
	#define LUCORE_ASSERT(expr, format, ...)                                                    \
		if(!(expr)) [[unlikely]] {                                                              \
			char buffer[256];                                                                   \
			std::snprintf(&buffer[0], sizeof(buffer),                                           \
						  "[Lucore] Assertion \"%s\" failed with message: " format "\n", #expr, \
						  ##__VA_ARGS__);                                                       \
			::lucore::ExitMsg(__FILE__, __LINE__, &buffer[0]);                                  \
		}
#else
	#define LUCORE_ASSERT(expr, format, ...)
#endif

// CHECK() is always active, even in release builds
#define LUCORE_CHECK(expr, format, ...)                                                 \
	if(!(expr)) [[unlikely]] {                                                          \
		char buffer[256];                                                               \
		std::snprintf(&buffer[0], sizeof(buffer),                                       \
					  "[Lucore] Check \"%s\" failed with message: " format "\n", #expr, \
					  ##__VA_ARGS__);                                                   \
		::lucore::ExitMsg(__FILE__, __LINE__, &buffer[0]);                              \
	}
