#include "LuaDevice.hpp"

LUA_CLASS_BIND_VARIABLES_IMPLEMENT(LuaDevice);

void LuaDevice::Bind(GarrysMod::Lua::ILuaBase* LUA) {
	// clang-format off
	LUA_CLASS_BIND_BEGIN(LuaDevice);
		// todo handlers
	LUA_CLASS_BIND_END();
	// clang-format on
}

void LuaDevice::Create(GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushUserType(new LuaDevice(), __lua_typeid);
}
