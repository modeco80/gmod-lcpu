//! Helper macros and inlines for binding Lua and C++.
//!
//! If you want to bind a C++ class to Lua, see the
//! [lcpu::lua::LuaObject<TImpl>] type in LuaObject.hpp.
#pragma once

#include <GarrysMod/Lua/Interface.h>

#include <lucore/Logger.hpp>

/// Declare a Lua function inside of a class.
/// This is like the official GMOD header's LUA_FUNCTION but allows:
/// - forward declaration
///	- being put inside of a class
/// - an out-of-line implementation (in .cpp file)
///
/// This makes writing class bindings a lot less annoying.
#define LUA_CLASS_FUNCTION_DECL(FUNC)                                \
	static int FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA); \
	static int FUNC(lua_State* L) {                              \
		GarrysMod::Lua::ILuaBase* LUA = L->luabase;              \
		LUA->SetState(L);                                        \
		return FUNC##__ImpStatic(LUA);                           \
	}

/// Implement a previously declared (with [LUA_CLASS_FUNCTION_DECL]) Lua function
/// from a class.
#define LUA_CLASS_FUNCTION(CLASS, FUNC) int CLASS::FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA)

/// Set a C function as a field of a table. 
/// The stack layout prior to this function should be:
/// -1 (top) -> table
#define LUA_SET_C_FUNCTION(name) \
	LUA->PushCFunction(name);    \
	LUA->SetField(-2, #name);


/// Set a C function as a field of a table, with an alternative name. 
/// The stack layout prior to this function should be:
/// -1 (top) -> table
#define LUA_SET_C_FUNCTION_NAME(name, altName) \
	LUA->PushCFunction(name);                  \
	LUA->SetField(-2, altName);

namespace lcpu::lua {

	/// Get a string from Lua as a STL string.
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
