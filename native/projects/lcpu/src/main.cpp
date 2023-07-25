#include <GarrysMod/Lua/Interface.h>

#include <lucore/Logger.hpp>
#include <lucore/Types.hpp>
#include "SourceSink.hpp"

#include "LcpuGlobals.hpp"

GMOD_MODULE_OPEN() {
	lucore::Logger::The().AttachSink(lcpu::SourceSink::The());
	lucore::LogInfo("LCPU Native Module!");
	GlobalsBind(LUA);
	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}
