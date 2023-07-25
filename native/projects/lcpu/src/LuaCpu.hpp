#pragma once 

#include <riscv/System.hpp>

#include "LuaHelpers.hpp"

/// Bindings of [riscv::System] to Lua.
struct LuaCpu {
	/// Lua binding stuff
	static void Bind(GarrysMod::Lua::ILuaBase* LUA);
	static void Create(GarrysMod::Lua::ILuaBase* LUA, u32 memorySize);

   private:

	LuaCpu(u32 memorySize);
	~LuaCpu();

	LUA_MEMBER_FUNCTION(PoweredOn);	   // Check if the CPU is powered on
	LUA_MEMBER_FUNCTION(Cycle);		   // do a single cycle (called internally by LCPU entity)
	LUA_MEMBER_FUNCTION(PowerOff);	   // power off and reset the LCPU
	LUA_MEMBER_FUNCTION(PowerOn);	   // power on the LCPU
	LUA_MEMBER_FUNCTION(Reset);		   // reset the LCPU
	LUA_MEMBER_FUNCTION(AttachDevice); // attach a LuaDevice to this cpu

	// class binding stuff
	LUA_CLASS_BIND_VARIABLES(private);

	// member variables
	riscv::System* system;
	bool poweredOn;
};
