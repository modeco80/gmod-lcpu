#include "LcpuGlobals.hpp"

#include <GarrysMod/Lua/Interface.h>
#include "LuaCpu.hpp"
#include "LuaDevice.hpp"
#include "lucore/Logger.hpp"

namespace lcpu {
	LUA_FUNCTION(LCPUNative_CreateCPU) {
		LUA->CheckType(1, GarrysMod::Lua::Type::Number);
		auto memorySize = static_cast<u32>(LUA->GetNumber(1));

		// TODO: There's probably a way to like, ensure a per-player max.
		if(memorySize > (64 * 1024 * 1024))
			LUA->ThrowError("Over RAM size limit.");

		lucore::LogDebug("Creating Lua CPU object with 0x{:08x} memory size", memorySize);
		LuaCpu::Create(LUA, memorySize);
		return 1;
	}

	LUA_FUNCTION(LCPUNative_CreateDevice) {
		auto base = LUA->CheckNumber(1);
		auto size = LUA->CheckNumber(2);
		lucore::LogDebug("Creating Lua device object mapped @ 0x{:08x} with size 0x{:08x}", static_cast<riscv::Address>(base),
						static_cast<riscv::Address>(size));
		LuaDevice::Create(LUA, static_cast<riscv::Address>(base), static_cast<riscv::Address>(size));
		return 1;
	}

	LUA_FUNCTION(LCPUNative_EnableDebug) {
		lucore::LogInfo("Enabling debug logging");
		lucore::Logger::The().SetLogLevel(lucore::Logger::MessageSeverity::Debug);
		return 0;
	}

	LUA_FUNCTION(LCPUNative_DisableDebug) {
		lucore::LogInfo("Disabling debug logging");
		lucore::Logger::The().SetLogLevel(lucore::Logger::MessageSeverity::Info);
		return 0;
	}

	void GlobalsBind(GarrysMod::Lua::ILuaBase* LUA) {
		LuaCpu::RegisterClass(LUA);
		LuaDevice::RegisterClass(LUA);

		// clang-format off
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->CreateTable();
			LUA->PushNumber(LCPU_MODULE_VERSION);
			LUA->SetField(-2, "ModuleVersion");

			LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateCPU, "CreateCPU");		/// Create a CPU
			LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateDevice, "CreateDevice"); /// Create a device implemented in Lua
			LUA_SET_C_FUNCTION_NAME(LCPUNative_EnableDebug, "EnableDebug");	/// Enable native module debug logging
			LUA_SET_C_FUNCTION_NAME(LCPUNative_DisableDebug, "DisableDebug");	/// Disable native module debug logging
		LUA->SetField(-2, "LCPUNative");
		LUA->Pop();
		// clang-format on
	}

} // namespace lcpu
