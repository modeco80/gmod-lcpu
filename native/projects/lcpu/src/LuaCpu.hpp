#include <riscv/System.hpp>

#include "LuaMember.hpp"

// A work-in-progress
struct LuaCpu {
	/// Lua binding stuff
	static void Bind(GarrysMod::Lua::ILuaBase* LUA);
	static void Create(GarrysMod::Lua::ILuaBase* LUA, u32 memorySize);

	LuaCpu(u32 memorySize);
	~LuaCpu();

   private:
	void CycleImpl();

	void OnSysconShutdown();

	// LUA user type id.
	static int __lua_typeid;

	// Metafunctions
	LUA_MEMBER_FUNCTION_DECLARE(__gc)
	LUA_MEMBER_FUNCTION_DECLARE(__tostring)

	// Called by the LCPU entity for specific tasks:
	LUA_MEMBER_FUNCTION_DECLARE(Cycle)
	LUA_MEMBER_FUNCTION_DECLARE(SetMemorySize)

	// member variables
	riscv::System* system;
	bool poweredOn = false;
};
