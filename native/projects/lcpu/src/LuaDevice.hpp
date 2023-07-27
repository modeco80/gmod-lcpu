#pragma once

#include <riscv/Bus.hpp>

#include "LuaHelpers.hpp"

/// A work-in-progress binding of [riscv::Bus::MmioDevice] to lua
struct LuaDevice : public riscv::Bus::MmioDevice {
	/// Lua binding stuff
	static void Bind(GarrysMod::Lua::ILuaBase* LUA);
	static void Create(GarrysMod::Lua::ILuaBase* LUA, riscv::Address base, riscv::Address size);

	~LuaDevice();

	bool Clocked() const override;
	void Clock() override;
	void Reset() override;

	riscv::Address Base() const override;
	riscv::Address Size() const override;

	u32 Peek(riscv::Address address) override;
	void Poke(riscv::Address address, u32 value) override;

   private:
	// class binding stuff
	LUA_CLASS_BIND_VARIABLES(private);

	LUA_MEMBER_FUNCTION(__index);
	LUA_MEMBER_FUNCTION(__newindex); 

	LuaDevice(riscv::Address base, riscv::Address size);

	riscv::Address base {};
	riscv::Address size {};
	GarrysMod::Lua::ILuaBase* LuaState;

	// this should be a common type tbh
	int tableReference = -1;
};
