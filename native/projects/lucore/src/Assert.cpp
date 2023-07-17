//! Implementation of lucore assertion facilities

#include <cstdio>
#include <cstdlib>
#include <lucore/Logger.hpp>

namespace lucore {

	[[noreturn]] void ExitMsg(const char* fileName, int fileLine, const char* message) {
		Logger::The().Fatal("{}", message);
		std::quick_exit(0xAF);
	}

} // namespace lucore
