#include "LuaDevice.hpp"

#include "LuaHelpers.hpp"

LUA_CLASS_BIND_VARIABLES_IMPLEMENT(LuaDevice);

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaDevice, SetBase) {
	auto self = LUA_CLASS_GET(LuaDevice)(1);
	if(self->bus)
		LUA->ThrowError("Do not call this on an attached device");
	self->base = static_cast<u32>(LUA->CheckNumber(2));
	return 0;
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaDevice, SetSize) {
	auto self = LUA_CLASS_GET(LuaDevice)(1);
	if(self->bus)
		LUA->ThrowError("Do not call this on an attached device");
	self->size = static_cast<u32>(LUA->CheckNumber(2));
	return 0;
}

bool LuaDevice::Clocked() const {
	// Even if Lua devices may not have a clock handler
	// installed there's no real non-awful way to check,
	// so just lie and say yes always.
	return true;
}

void LuaDevice::Clock() {
	LuaState->PushUserType(this, __lua_typeid);
	LuaState->GetField(-2, "Clock");
	if(!LuaState->IsType(-1, GarrysMod::Lua::Type::Function)) {
		LuaState->Pop(2);
		return;
	}
	LuaState->Push(-2); // "self"
	LuaState->Call(1, 0);
	LuaState->Pop();
}

riscv::Address LuaDevice::Base() const {
	return base;
}

riscv::Address LuaDevice::Size() const {
	return base;
}

u32 LuaDevice::Peek(riscv::Address address) {
	LuaState->PushUserType(this, __lua_typeid);
	LuaState->GetField(-2, "Peek");
	if(!LuaState->IsType(-1, GarrysMod::Lua::Type::Function)) {
		LuaState->Pop(2);
		return -1;
	}
	LuaState->Push(-2); // "self"
	LuaState->PushNumber(address);
	LuaState->Call(2, 1);
	auto result = LuaState->GetNumber(-1);
	LuaState->Pop();
	return static_cast<u32>(result);
}

void LuaDevice::Poke(riscv::Address address, u32 value) {
	LuaState->PushUserType(this, __lua_typeid);
	LuaState->GetField(-2, "Poke");
	if(!LuaState->IsType(-1, GarrysMod::Lua::Type::Function)) {
		LuaState->Pop(2);
		return;
	}
	LuaState->Push(-2); // "self"
	LuaState->PushNumber(address);
	LuaState->PushNumber(value);
	LuaState->Call(3, 0);
	LuaState->Pop();
}

void LuaDevice::Reset() {
	LuaState->PushUserType(this, __lua_typeid);
	LuaState->GetField(-2, "Reset");
	if(!LuaState->IsType(-1, GarrysMod::Lua::Type::Function))
		return;

	LuaState->Push(-2); // "self"
	LuaState->Call(1, 0);
	LuaState->Pop();
}

LuaDevice::~LuaDevice() = default;

void LuaDevice::Bind(GarrysMod::Lua::ILuaBase* LUA) {
	// clang-format off
	// TODO: I need to figure out how to like, set up metamethod stuff
	// so it all properly works.
	LUA_CLASS_BIND_BEGIN(LuaDevice);
		LUA_SET_C_FUNCTION(SetBase)
		LUA_SET_C_FUNCTION(SetSize)
	LUA_CLASS_BIND_END();
	// clang-format on
}

void LuaDevice::Create(GarrysMod::Lua::ILuaBase* LUA) {
	auto device = new LuaDevice();
	device->LuaState = LUA;
	LUA->PushUserType(device, __lua_typeid);
}
