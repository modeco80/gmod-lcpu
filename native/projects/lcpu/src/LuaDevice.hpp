#pragma once

#include <riscv/Bus.hpp>

#include "LuaHelpers.hpp"
#include "LuaObject.hpp"

/// A work-in-progress binding of [riscv::Bus::MmioDevice] to lua
struct LuaDevice : public riscv::Bus::MmioDevice, lcpu::lua::LuaObject<LuaDevice> {
	/// Lua binding stuff
	constexpr static const char* Name() { return "LuaDevice"; }
	static void RegisterClass(GarrysMod::Lua::ILuaBase* LUA);

	// [riscv::Bus::MmioDevice] implementation
	bool Clocked() const override;
	void Clock() override;
	void Reset() override;

	riscv::Address Base() const override;
	riscv::Address Size() const override;

	u32 Peek(riscv::Address address) override;
	void Poke(riscv::Address address, u32 value) override;

   protected:
	friend lcpu::lua::LuaObject<LuaDevice>;
	LuaDevice(riscv::Address base, riscv::Address size);
	~LuaDevice() = default;

   private:
	riscv::Address base {};
	riscv::Address size {};
};