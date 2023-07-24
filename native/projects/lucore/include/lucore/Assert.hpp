//! Lucore Assert Wrappers

#pragma once

#include <format>

namespace lucore {
	[[noreturn]] void ExitMsg(const char* message);
} // namespace lucore

#ifndef NDEBUG
	#define LUCORE_ASSERT(expr, fmt, ...)                                                                                                           \
		if(!(expr)) [[unlikely]] {                                                                                                                  \
			auto msg = std::format("Assertion \"{}\" @ {}:{} failed with message: {}", #expr, __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__)); \
			::lucore::ExitMsg(msg.c_str());                                                                                                         \
		}
#else
	#define LUCORE_ASSERT(expr, format, ...)
#endif

// CHECK() is always active, even in release builds
#define LUCORE_CHECK(expr, fmt, ...)                                                                                                        \
	if(!(expr)) [[unlikely]] {                                                                                                              \
		auto msg = std::format("Check \"{}\" @ {}:{} failed with message: {}", #expr, __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__)); \
		::lucore::ExitMsg(msg.c_str());                                                                                                     \
	}
