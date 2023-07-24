#include <GarrysMod/Lua/Interface.h>

#include <cmath>
#include <lucore/Logger.hpp>
#include <lucore/Types.hpp>

#include "SourceSink.hpp"

#include "LuaCpu.hpp"


LUA_FUNCTION(LCPUNative_CreateCPU) {
	LUA->CheckType(1, GarrysMod::Lua::Type::Number);
	auto memorySize = (u32)std::round(LUA->GetNumber(1));

	if (memorySize > (64 * 1024 * 1024)) {
		LUA->ThrowError("Over current RAM size limit.");
	}

	//LuaCpu::Create(LUA, memorySize);
	return 0;
}

// Create a device object. This is internally used by the CPU to create Wire and other interface things.
LUA_FUNCTION(LCPUNative_CreateDevice) {
	return 0; // for now, while LuaDevice doesn't exist?
}

void LCPUNative_Bind(GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
	LUA->CreateTable();
		LUA->PushNumber(1);
		LUA->SetField(-2, "ModuleVersion");

		LUA->PushCFunction(LCPUNative_CreateCPU);
		LUA->SetField(-2, "CreateCPU");

		LUA->PushCFunction(LCPUNative_CreateDevice);
		LUA->SetField(-2, "CreateDevice");
	LUA->SetField(-2, "LCPUNative");
	LUA->Pop();
}



GMOD_MODULE_OPEN() {
	lucore::Logger::The().AttachSink(lcpu::SourceSink::The());

	lucore::LogInfo("LCPU Native Module!");


	// Let lua types bind
	//LuaCpu::Bind(LUA);

	// Bind the global namespace
	LCPUNative_Bind(LUA);


	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}
