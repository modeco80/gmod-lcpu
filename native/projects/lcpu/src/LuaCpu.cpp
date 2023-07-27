#include "LuaCpu.hpp"

#include <lucore/Logger.hpp>

#include "LuaDevice.hpp"

// this is temporary from the test harness, and will be replaced
// at some point.
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

namespace lcpu {
	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, PoweredOn) {
		auto self = LuaCpu::FromLua(LUA, 1);
		LUA->PushBool(self->poweredOn);
		return 1;
	}

	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, Cycle) {
		auto self = LuaCpu::FromLua(LUA, 1);
		if(!self->poweredOn)
			return 0;
		self->system->Step();
		return 0;
	}

	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, PowerOff) {
		auto self = LuaCpu::FromLua(LUA, 1);
		if(!self->poweredOn)
			return 0;

		self->poweredOn = false;
		self->system->bus->Reset();
		return 0;
	}

	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, PowerOn) {
		auto self = LuaCpu::FromLua(LUA, 1);
		if(self->poweredOn)
			return 0;

		self->poweredOn = true;
		self->system->bus->Reset();
		return 0;
	}

	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, Reset) {
		auto self = LuaCpu::FromLua(LUA, 1);
		self->system->bus->Reset();
		return 0;
	}

	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaCpu, AttachDevice) {
		auto self = LuaCpu::FromLua(LUA, 1);
		auto device = LuaDevice::FromLua(LUA, 2);

		// Attach it
		LUA->PushBool(self->system->bus->AttachDevice(static_cast<riscv::Bus::Device*>(device)));
		return 1;
	}

	void LuaCpu::RegisterClass(GarrysMod::Lua::ILuaBase* LUA) {
		RegisterClassStart(LUA);

		RegisterMethod("PoweredOn", PoweredOn);
		RegisterMethod("Cycle", Cycle);
		RegisterMethod("PowerOff", PowerOff);
		RegisterMethod("PowerOn", PowerOn);
		RegisterMethod("Reset", Reset);
		RegisterMethod("AttachDevice", AttachDevice);
	}

	LuaCpu::LuaCpu(u32 memorySize) {
		poweredOn = true;
		system = riscv::System::Create(memorySize);
		system->OnPowerOff = [&]() {
			poweredOn = false;
			system->bus->Reset();
		};

		// lame test code. this WILL be removed, I just want this for a quick test
		system->bus->AttachDevice(new SimpleUartDevice);
		auto fp = std::fopen("/home/lily/test-gmod.bin", "rb");
		if(fp) {
			std::fseek(fp, 0, SEEK_END);
			auto len = std::ftell(fp);
			std::fseek(fp, 0, SEEK_SET);

			std::fread(system->ram->Raw(), 1, len, fp);
			std::fclose(fp);
		}
	}

	LuaCpu::~LuaCpu() {
		delete system;
	}

} // namespace lcpu
