#pragma once

#include <GarrysMod/Lua/Interface.h>

// These are like the official GMOD header LUA_FUNCTION but allow forward declaration
// and implementation inside of classes. Pretty nifty!
#define LUA_MEMBER_FUNCTION_DECLARE(FUNC)                          \
	static int FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA); \
	static int FUNC(lua_State* L) {                        \
		GarrysMod::Lua::ILuaBase* LUA = L->luabase;        \
		LUA->SetState(L);                                  \
		return FUNC##__ImpStatic(LUA);                           \
	}

#define LUA_MEMBER_FUNCTION(CLASS, FUNC) int CLASS::FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA) 
