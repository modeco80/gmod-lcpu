//! Lucore Assert Wrappers

#pragma once

#include <format>

namespace lucore {

	// TODO: wrapper which uses source_location (we don't need no macros anymore!)
	[[noreturn]] void ExitMsg(const char* fileName, int fileLine, const char* message);

} // namespace lucore

#ifndef NDEBUG
	#define LUCORE_ASSERT(expr, fmt, ...)                                             \
		if(!(expr)) [[unlikely]] {                                                    \
			auto msg = std::format("Assertion \"{}\" failed with message: {}", #expr, \
								   std::format(fmt, ##__VA_ARGS__));                  \
			::lucore::ExitMsg(__FILE__, __LINE__, msg.c_str());                       \
		}
#else
	#define LUCORE_ASSERT(expr, format, ...)
#endif

// CHECK() is always active, even in release builds
#define LUCORE_CHECK(expr, fmt, ...)                                          \
	if(!(expr)) [[unlikely]] {                                                \
		auto msg = std::format("Check \"{}\" failed with message: {}", #expr, \
							   std::format(fmt, ##__VA_ARGS__));              \
		::lucore::ExitMsg(__FILE__, __LINE__, msg.c_str());                   \
	}
