//! Loader/types for lua_shared API. I plan to use this instead of the Garry's Mod
//! ILuaBase interface; they are interchangable (and in fact, less friction to use!)

#pragma once

#include <GarrysMod/Lua/Interface.h>

namespace lcpu::lua {

#define LUA_IDSIZE 60 /* Size of lua_Debug.short_src. */

	struct lua_Debug {
		int event;
		const char* name;			/* (n) */
		const char* namewhat;		/* (n) `global', `local', `field', `method' */
		const char* what;			/* (S) `Lua', `C', `main', `tail' */
		const char* source;			/* (S) */
		int currentline;			/* (l) */
		int nups;					/* (u) number of upvalues */
		int linedefined;			/* (S) */
		int lastlinedefined;		/* (S) */
		char short_src[LUA_IDSIZE]; /* (S) */
		/* private part */
		int i_ci; /* active function */
	};

	typedef int (*lua_CFunction)(lua_State* L);
	typedef void* (*lua_Alloc)(void* ud, void* ptr, size_t osize, size_t nsize);
	typedef const char* (*lua_Reader)(lua_State* L, void* ud, size_t* sz);
	typedef int (*lua_Writer)(lua_State* L, const void* p, size_t sz, void* ud);
	//typedef struct lua_Debug lua_Debug; /* activation record */
	typedef void (*lua_Hook)(lua_State* L, lua_Debug* ar);

	typedef struct luaL_Buffer {
		char* p; /* current position in buffer */
		int lvl; /* number of strings in the stack (level) */
		lua_State* L;
		char buffer[8192];
	} luaL_Buffer;

	typedef struct luaL_Reg {
		const char* name;
		lua_CFunction func;
	} luaL_Reg;

#define LUA_SHARED_FUNC(Retty, name, ...) extern Retty (*name)(__VA_ARGS__);
	#include "LuaSharedFunctions.inc"
#undef LUA_SHARED_FUNC

	/// Load lua_shared
	void LoadLuaShared();

	/// .. and unload it.
	void UnloadLuaShared();

} // namespace lcpu::lua
