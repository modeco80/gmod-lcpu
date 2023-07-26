#include "LuaShared.hpp"

#include <lucore/Assert.hpp>
#include <lucore/Library.hpp>

namespace lcpu::lua {

	// A global pointer to the loaded instance of the lua_shared library.
	lucore::Library* luaSharedLibrary = nullptr;

#define LUA_SHARED_FUNC(Retty, name, ...) Retty (*name)(__VA_ARGS__) = nullptr;
#include "LuaSharedFunctions.inc"
#undef LUA_SHARED_FUNC

	void LoadLuaShared() {
		// Open the lua_shared library
		luaSharedLibrary = lucore::Library::OpenExisting("lua_shared");
		LUCORE_CHECK(luaSharedLibrary != nullptr, "Could not open lua_shared library for some reason.");

		// Load the functions from lua_shared
		// This is helped quite a bit by using an x-macro
#define LUA_SHARED_FUNC(Retty, name, ...)                   \
	name = luaSharedLibrary->Symbol<decltype(name)>(#name); \
	LUCORE_CHECK(name != nullptr, "Could not load lua_shared function \"{}\"", #name);
#include "LuaSharedFunctions.inc"
#undef LUA_SHARED_FUNC
	}

	void UnloadLuaShared() {
		LUCORE_ASSERT(luaSharedLibrary != nullptr, "UnloadLuaShared() should only be called if LoadLuaShared() was called");
		delete luaSharedLibrary;

#define LUA_SHARED_FUNC(Retty, name, ...) name = nullptr;
#include "LuaSharedFunctions.inc"
#undef LUA_SHARED_FUNC
	}

} // namespace lcpu::lua
