#include <GarrysMod/Lua/Interface.h>

#include <lucore/Assert.hpp>

#include "SourceSink.hpp"

LUA_FUNCTION(lcpu_native_test) {
}

GMOD_MODULE_OPEN() {
	lucore::Logger::The().AttachSink(lcpu::SourceSink::The());

	lucore::LogInfo("LCPU Native Module loading");

	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}
