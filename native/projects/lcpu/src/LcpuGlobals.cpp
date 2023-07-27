#include "LcpuGlobals.hpp"

#include <GarrysMod/Lua/Interface.h>
#include "LuaCpu.hpp"
#include "LuaDevice.hpp"

namespace lcpu {
	LUA_FUNCTION(LCPUNative_CreateCPU) {
		LUA->CheckType(1, GarrysMod::Lua::Type::Number);
		auto memorySize = static_cast<u32>(LUA->GetNumber(1));

		// TODO: There's probably a way to like, ensure a per-player max.
		if(memorySize > (64 * 1024 * 1024))
			LUA->ThrowError("Over RAM size limit.");

		LuaCpu::Create(LUA, memorySize);
		return 1;
	}

	LUA_FUNCTION(LCPUNative_CreateDevice) {
		auto base = LUA->CheckNumber(1);
		auto size = LUA->CheckNumber(2);
		lucore::LogInfo("Creating Lua device object mapped @ 0x{:08x} with size 0x{:08x}", static_cast<riscv::Address>(base),
						static_cast<riscv::Address>(size));
		LuaDevice::Create(LUA, static_cast<riscv::Address>(base), static_cast<riscv::Address>(size));
		return 1;
	}

	void GlobalsBind(GarrysMod::Lua::ILuaBase* LUA) {
		LuaCpu::RegisterClass(LUA);
		LuaDevice::RegisterClass(LUA);

		// clang-format off
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->CreateTable();
			LUA->PushNumber(LCPU_MODULE_VERSION);
			LUA->SetField(-2, "ModuleVersion");

			LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateCPU, "CreateCPU");
			LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateDevice, "CreateDevice");
		LUA->SetField(-2, "LCPUNative");
		LUA->Pop();
		// clang-format on
	}

} // namespace lcpu
