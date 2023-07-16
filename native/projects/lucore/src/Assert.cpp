//! Implementation of lucore assertion facilities

#include <cstdio>
#include <cstdlib>

namespace lucore {

	[[noreturn]] void ExitMsg(const char* fileName, int fileLine, const char* message) {
		// TODO: move this to logger functionality of lucore (the native module will end up
		// containing a Sink implementation that funnels to either gmod or tier libs..)
		std::puts(message);
		std::quick_exit(0xAF);
	}

} // namespace lucore
