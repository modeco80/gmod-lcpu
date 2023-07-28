#pragma once

#include <riscv/System.hpp>

#include "LuaObject.hpp"

namespace lcpu {
	/// Binding of [riscv::System] to Lua.
	struct LuaCpu : public lua::LuaObject<LuaCpu> {
		/// Lua binding stuff
		constexpr static const char* Name() { return "LuaCpu"; }
		static void RegisterClass(GarrysMod::Lua::ILuaBase* LUA);

	   protected:
		friend struct lua::LuaObject<LuaCpu>;
		LuaCpu(u32 memorySize);
		~LuaCpu();

	   private:
		LUA_CLASS_FUNCTION_DECL(PoweredOn);	   // Check if the CPU is powered on
		LUA_CLASS_FUNCTION_DECL(Cycle);		   // do a single cycle (called internally by LCPU entity)
		LUA_CLASS_FUNCTION_DECL(PowerOff);	   // power off and reset the LCPU
		LUA_CLASS_FUNCTION_DECL(PowerOn);	   // power on the LCPU
		LUA_CLASS_FUNCTION_DECL(Reset);		   // reset the LCPU
		LUA_CLASS_FUNCTION_DECL(AttachDevice); // attach a LuaDevice to this cpu

		// member variables
		riscv::System* system;
		bool poweredOn;
	};

} // namespace lcpu
