#include "LuaCpu.hpp"

#include <lucore/Logger.hpp>

#include "LuaDevice.hpp"
#include "LuaHelpers.hpp"

// this is temporary from the thing

/// simple 16550 UART implementation
struct SimpleUartDevice : public riscv::Bus::MmioDevice {
	constexpr static riscv::Address BASE_ADDRESS = 0x10000000;

	riscv::Address Base() const override { return BASE_ADDRESS; }
	riscv::Address Size() const override { return 12; } // for now

	u32 Peek(riscv::Address address) override {
		switch(address) {
			case BASE_ADDRESS: return '\0';		// just return 0 for the input register
			case BASE_ADDRESS + 5: return 0x60; // active, but no keyboard input
		}

		return 0;
	}

	void Poke(riscv::Address address, u32 value) override {
		if(address == BASE_ADDRESS) {
			char c = value & 0x000000ff;
			std::fputc(c, stderr);
		}
	}
};

LUA_CLASS_BIND_VARIABLES_IMPLEMENT(LuaCpu);

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, PoweredOn) {
	LUA_CLASS_GET(LuaCpu, self, 1);
	LUA->PushBool(self->poweredOn);
	return 1;
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, Cycle) {
	LUA_CLASS_GET(LuaCpu, self, 1);
	[&self]() {
		if(!self->poweredOn)
			return;
		self->system->Step();
	}();
	return 0;
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, PowerOff) {
	LUA_CLASS_GET(LuaCpu, self, 1);
	[&self]() {
		if(!self->poweredOn)
			return;

		self->poweredOn = false;
		self->system->bus->Reset();
	}();

	return 0;
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, PowerOn) {
	LUA_CLASS_GET(LuaCpu, self, 1);
	[&self]() {
		if(self->poweredOn)
			return;

		self->poweredOn = true;
		self->system->bus->Reset();
	}();
	return 0;
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, Reset) {
	LUA_CLASS_GET(LuaCpu, self, 1);
	[&self]() { self->system->bus->Reset(); }();
	return 0;
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, AttachDevice) {
	LUA_CLASS_GET(LuaCpu, self, 1);
	bool result = false;
#if 0
	[&]() {
		LUA_CLASS_GET(LuaDevice, device, 2);
		if(!device)
			return; // the bus is safe against this possibility, but
					// I'd rather be doubly-safe tbh

		// Attach it
		result = self->system->bus->AttachDevice(static_cast<riscv::Bus::Device*>(device));
	}();
#endif
	LUA->PushBool(result);
	return 1;
}

void LuaCpu::Bind(GarrysMod::Lua::ILuaBase* LUA) {
	// clang-format off
	LUA_CLASS_BIND_BEGIN(LuaCpu);
			LUA_SET_C_FUNCTION(PoweredOn);
			LUA_SET_C_FUNCTION(Cycle);
			LUA_SET_C_FUNCTION(PowerOff);
			LUA_SET_C_FUNCTION(PowerOn);
			LUA_SET_C_FUNCTION(Reset);
			LUA_SET_C_FUNCTION(AttachDevice);
	LUA_CLASS_BIND_END();
	// clang-format on
}

void LuaCpu::Create(GarrysMod::Lua::ILuaBase* LUA, u32 memorySize) {
	auto cpuWrapper = new LuaCpu(memorySize);

	// lame test code. this WILL be removed, I just want this for a quick test
	cpuWrapper->system->bus->AttachDevice(new SimpleUartDevice);
	auto fp = std::fopen("/home/lily/test.bin", "rb");
	if(fp) {
		std::fseek(fp, 0, SEEK_END);
		auto len = std::ftell(fp);
		std::fseek(fp, 0, SEEK_SET);

		std::fread(cpuWrapper->system->ram->Raw(), 1, len, fp);
		std::fclose(fp);
	}

	LUA->PushUserType(cpuWrapper, __lua_typeid);
}

LuaCpu::LuaCpu(u32 memorySize) {
	lucore::LogInfo("in LuaCpu::LuaCpu(0x{:08x})\n", memorySize);

	poweredOn = true;
	system = riscv::System::Create(memorySize);
	system->OnPowerOff = [&]() {
		poweredOn = false;
		system->bus->Reset();
	};
}

LuaCpu::~LuaCpu() {
	delete system;
}
