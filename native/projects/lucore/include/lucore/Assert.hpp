//! Lucore Assert Wrappers
//!
//! Lucore uses its own assertion system which is more flexible than the
//! standard C library's assertion macros.
//!
//! They are not intended to be directly compatible with some of the quirks
//! the Standard C library allows (like using assert() as an expression).
//!
//! They are:
//! - LUCORE_ASSERT() 
//!		- active in debug builds and removed on release
//! - LUCORE_CHECK() 
//! 	- always active, even in release builds

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

#define LUCORE_CHECK(expr, fmt, ...)                                                                                                        \
	if(!(expr)) [[unlikely]] {                                                                                                              \
		auto msg = std::format("Check \"{}\" @ {}:{} failed with message: {}", #expr, __FILE__, __LINE__, std::format(fmt, ##__VA_ARGS__)); \
		::lucore::ExitMsg(msg.c_str());                                                                                                     \
	}
