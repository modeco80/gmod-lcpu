//! Helpers for lua binding
#pragma once

#include <GarrysMod/Lua/Interface.h>
#include <lucore/Logger.hpp>

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

// will make a "self" variable with the class
#define LUA_CLASS_GET(T, name, stackPos)                                                      \
	LUA->CheckType(stackPos, T::__lua_typeid);                                                \
	auto name = LUA->GetUserType<T>(stackPos, T::__lua_typeid);                               \
	if(!name) {                                                                               \
		LUA->ThrowError("nullptr " #T " passed to function which requires a valid instance"); \
	}

#define LUA_CLASS_BIND_VARIABLES(ACCESS_LEVEL) \
   public:                                     \
	static int __lua_typeid;                   \
	ACCESS_LEVEL:                              \
	LUA_MEMBER_FUNCTION(__gc);

#define LUA_CLASS_BIND_VARIABLES_IMPLEMENT(T)                              \
	int T::__lua_typeid = 0;                                               \
	LUA_MEMBER_FUNCTION_IMPLEMENT(T, __gc) {                               \
		LUA->CheckType(1, T::__lua_typeid);                                \
		auto self = LUA->GetUserType<T>(1, T::__lua_typeid);               \
		if(self != nullptr) { /* GetUserType returns nullptr on failure */ \
			lucore::LogInfo("GCing {} object", #T);                        \
			delete self;                                                   \
		}                                                                  \
		return 0;                                                          \
	}

#define LUA_CLASS_BIND_BEGIN(T)                    \
	T::__lua_typeid = LUA->CreateMetaTable(#T);    \
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG); \
	LUA->PushNumber(T::__lua_typeid);              \
	LUA->SetField(-2, #T "__typeid");              \
	LUA->Pop(); /* pop registry */                 \
	LUA->Push(-1);                                 \
	LUA->SetField(-2, "__index");                  \
	LUA_SET_C_FUNCTION(__gc)

#define LUA_CLASS_BIND_END() LUA->Pop();

// Helpers for lua functions
#define LUA_SET_C_FUNCTION(name) \
	LUA->PushCFunction(name);    \
	LUA->SetField(-2, #name);

#define LUA_SET_C_FUNCTION_NAME(name, altName) \
	LUA->PushCFunction(name);                  \
	LUA->SetField(-2, altName);
