#include "LcpuGlobals.hpp"

#include "GarrysMod/Lua/Interface.h"
#include "GarrysMod/Lua/LuaBase.h"
#include "LuaCpu.hpp"
#include "LuaDevice.hpp"

#include "LuaHelpers.hpp"
#include "LuaObject.hpp"

/// test for the "new" lua object system
struct TestLuaObject : public lcpu::lua::LuaObject<TestLuaObject> {
	constexpr static const char* Name() { return "TestLuaObject"; }

	static void RegisterClass(GarrysMod::Lua::ILuaBase* LUA) {
		RegisterClassStart(LUA);

		// Register methods. Maybe later I'll do some crazy template stuff; for now this is pretty barebones.
		RegisterMethod("Test", Test);
		RegisterGetter("Variable", [](GarrysMod::Lua::ILuaBase* LUA) { LUA->PushNumber(32.6); });
		RegisterGetter("MemberVariable", [](GarrysMod::Lua::ILuaBase* LUA) {
			auto self = TestLuaObject::FromLua(LUA, 1);
			LUA->PushNumber(self->n);
		});
		RegisterSetter("MemberVariable", [](GarrysMod::Lua::ILuaBase* LUA) {
			// The value of a setter is placed at the top of the stack by LuaObject
			auto self = TestLuaObject::FromLua(LUA, 1);
			self->n = LUA->GetNumber(-1);
		});
	}

	LUA_MEMBER_FUNCTION(Test);
	double n;
};

LUA_MEMBER_FUNCTION_IMPLEMENT(TestLuaObject, Test) {
	LUA->PushString("hi :)");
	return 1;
}

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

 LUA_FUNCTION(LCPUNative_CreateTest) {
	TestLuaObject::Create(LUA);
	return 1;
}

void GlobalsBind(GarrysMod::Lua::ILuaBase* LUA) {
	LuaCpu::RegisterClass(LUA);
	LuaDevice::RegisterClass(LUA);
	TestLuaObject::RegisterClass(LUA);

	// clang-format off
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->CreateTable();
		LUA->PushNumber(LCPU_MODULE_VERSION);
		LUA->SetField(-2, "ModuleVersion");

		LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateCPU, "CreateCPU");
		LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateDevice, "CreateDevice");

		LUA_SET_C_FUNCTION_NAME(LCPUNative_CreateTest, "CreateTest");
	LUA->SetField(-2, "LCPUNative");
	LUA->Pop();
	// clang-format on
}
