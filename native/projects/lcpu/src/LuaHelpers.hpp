//! Helpers for lua binding
#pragma once

#include <GarrysMod/Lua/Interface.h>

#include <lucore/Logger.hpp>
#include <unordered_map>

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

// this synthesizes a lambda which takes the stack argument to get. this can actually also be
// stored as a variable for later usage (... if you so desire?)
#define LUA_CLASS_GET(T)                                       \
	[LUA](int stackPos) {                                      \
		LUA->CheckType(stackPos, T::__lua_typeid);             \
		return LUA->GetUserType<T>(stackPos, T::__lua_typeid); \
	}

// This class binding package always implements the __gc metamethod
// to free any C++ object bound to Lua.

// Declare required binding variables.
#define LUA_CLASS_BIND_VARIABLES(ACCESS_LEVEL) \
   public:                                     \
	static int __lua_typeid;                   \
	ACCESS_LEVEL:                              \
	LUA_MEMBER_FUNCTION(__gc);

// Implement required binding variables (typically in a .cpp file).
#define LUA_CLASS_BIND_VARIABLES_IMPLEMENT(T)                                        \
	int T::__lua_typeid = 0;                                                         \
	LUA_MEMBER_FUNCTION_IMPLEMENT(T, __gc) {                                         \
		LUA->CheckType(1, T::__lua_typeid);                                          \
		auto self = LUA->GetUserType<T>(1, T::__lua_typeid);                         \
		if(self != nullptr) { /* GetUserType returns nullptr on failure */           \
			lucore::LogInfo("GCing {} object @ {:p}", #T, static_cast<void*>(self)); \
			delete self;                                                             \
		}                                                                            \
		return 0;                                                                    \
	}

// Begin the Bind() method of a class. This just sets up boilerplate
// and required things to setup a class.
#define LUA_CLASS_BIND_BEGIN(T)                    \
	T::__lua_typeid = LUA->CreateMetaTable(#T);    \
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG); \
	LUA->PushNumber(T::__lua_typeid);              \
	LUA->SetField(-2, #T "__typeid");              \
	LUA->Pop(); /* pop registry */                 \
	LUA->Push(-1);                                 \
	LUA->SetField(-2, "__index");                  \
	LUA_SET_C_FUNCTION(__gc)

// End the Bind() method.
#define LUA_CLASS_BIND_END() LUA->Pop();

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
