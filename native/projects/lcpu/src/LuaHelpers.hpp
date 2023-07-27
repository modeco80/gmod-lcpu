//! Helpers for binding Lua and C++.
//! If you want to bind a C++ class to Lua, see the
//! [lcpu::lua::LuaObject<TImpl>] type in LuaObject.hpp
#pragma once

#include <GarrysMod/Lua/Interface.h>

#include <lucore/Logger.hpp>

#include "GarrysMod/Lua/LuaBase.h"

// These are like the official GMOD header LUA_FUNCTION but allow forward declaration
// and implementation inside of classes, making writing class bindings that much less
// of a PITA. Nifty!
#define LUA_MEMBER_FUNCTION(FUNC)                                \
	static int FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA); \
	static int FUNC(lua_State* L) {                              \
		GarrysMod::Lua::ILuaBase* LUA = L->luabase;              \
		LUA->SetState(L);                                        \
		return FUNC##__ImpStatic(LUA);                           \
	}

#define LUA_MEMBER_FUNCTION_IMPLEMENT(CLASS, FUNC) int CLASS::FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA)

// Set a C function as a field.
#define LUA_SET_C_FUNCTION(name) \
	LUA->PushCFunction(name);    \
	LUA->SetField(-2, #name);

// Set a C function as a field with an alternative field name.
#define LUA_SET_C_FUNCTION_NAME(name, altName) \
	LUA->PushCFunction(name);                  \
	LUA->SetField(-2, altName);

namespace lcpu::lua {

	inline std::string GetLuaString(GarrysMod::Lua::ILuaBase* LUA, int stackPos) {
		unsigned len {};
		auto ptr = LUA->GetString(stackPos, &len);
		if(ptr) {
			return std::string(ptr, len);
		} else {
			return {};
		}
	}

} // namespace lcpu::lua
