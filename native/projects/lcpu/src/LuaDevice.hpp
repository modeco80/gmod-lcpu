#pragma once

#include <riscv/Bus.hpp>

#include "LuaHelpers.hpp"

/// A work-in-progress binding of [riscv::Bus::MmioDevice] to lua
struct LuaDevice : public riscv::Bus::MmioDevice {
	/// Lua binding stuff
	static void Bind(GarrysMod::Lua::ILuaBase* LUA);
	static void Create(GarrysMod::Lua::ILuaBase* LUA);

	riscv::Address Base() const override { return base; }
	riscv::Address Size() const override { return size; } // I think this is right?

	u32 Peek(riscv::Address address) override;
	void Poke(riscv::Address address, u32 value) override;

   private:
	// class binding stuff
	LUA_CLASS_BIND_VARIABLES(private);

	// Do not call these once attached to a bus.
	LUA_MEMBER_FUNCTION(SetBase);
	LUA_MEMBER_FUNCTION(SetSize);

	LUA_MEMBER_FUNCTION(SetClockHandler);
	LUA_MEMBER_FUNCTION(SetResetHandler);
	LUA_MEMBER_FUNCTION(SetPeekHandler);
	LUA_MEMBER_FUNCTION(SetPokeHandler);

	riscv::Address base {};
	riscv::Address size {};

	// ...?
	GarrysMod::Lua::ILuaBase* LuaState;
};
