#include <GarrysMod/Lua/Interface.h>

#include <lucore/Logger.hpp>
#include <lucore/Types.hpp>
#include "SourceSink.hpp"

#include "LcpuGlobals.hpp"

GMOD_MODULE_OPEN() {
	// Initialize our lua_shared binding. It is important we ca
	lucore::Logger::The().AttachSink(lcpu::SourceSink::The());
	lucore::LogInfo("LCPU Native Module! (ModuleVersion {})", LCPU_MODULE_VERSION);

	lcpu::GlobalsBind(LUA);
	return 0;
}

GMOD_MODULE_CLOSE()  {
	return 0;
}
