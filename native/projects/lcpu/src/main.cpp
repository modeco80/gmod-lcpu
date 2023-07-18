#include <GarrysMod/Lua/Interface.h>

#include "SourceSink.hpp"

#include <lucore/Assert.hpp>

GMOD_MODULE_OPEN() {
	// Initalize the source logging sink and attach it to the lucore logger.
	lcpu::SourceSink::Init();
	lucore::Logger::The().AttachSink(lcpu::SourceSink::The());

	lucore::LogInfo("Hello Source World :) {} {}", 123.456, "This should work");
	lucore::LogWarning("test");
	LUCORE_CHECK(false, "this should bring the process down");
	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}
