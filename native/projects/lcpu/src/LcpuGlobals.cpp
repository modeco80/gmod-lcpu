#include "LcpuGlobals.hpp"

#include <cmath>

#include "LuaCpu.hpp"
#include "LuaDevice.hpp"

/// This should be bumped on any incompatible change to the native bindings
/// that would break older Lua code, or requires newer Lua code to run.
#define LCPU_MODULE_VERSION 1

LUA_FUNCTION(LCPUNative_CreateCPU) {
	LUA->CheckType(1, GarrysMod::Lua::Type::Number);
	auto memorySize = (u32)std::round(LUA->GetNumber(1));

	// TODO: There's probably a way to like, ensure a per-player max.
	if(memorySize > (64 * 1024 * 1024)) {
		LUA->ThrowError("Over RAM size limit.");
	}

	LuaCpu::Create(LUA, memorySize);
	return 1;
}

LUA_FUNCTION(LCPUNative_CreateDevice) {
	//LuaDevice::Create(LUA);
	return 0;//1;
}

void GlobalsBind(GarrysMod::Lua::ILuaBase* LUA) {
	LuaCpu::Bind(LUA);
	//LuaDevice::Bind(LUA);

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
