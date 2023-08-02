#include "LuaCpu.hpp"

#include <lucore/Logger.hpp>

#include "LuaDevice.hpp"

namespace lcpu {
	LUA_CLASS_FUNCTION(LuaCpu, PoweredOn) {
		auto self = LuaCpu::FromLua(LUA, 1);
		LUA->PushBool(self->poweredOn);
		return 1;
	}

	LUA_CLASS_FUNCTION(LuaCpu, Cycle) {
		auto self = LuaCpu::FromLua(LUA, 1);
		if(!self->poweredOn)
			return 0;
		self->system->Step();
		return 0;
	}

	LUA_CLASS_FUNCTION(LuaCpu, PowerOff) {
		auto self = LuaCpu::FromLua(LUA, 1);
		if(!self->poweredOn)
			return 0;

		self->poweredOn = false;
		return 0;
	}

	LUA_CLASS_FUNCTION(LuaCpu, PowerOn) {
		auto self = LuaCpu::FromLua(LUA, 1);
		if(self->poweredOn)
			return 0;

		self->poweredOn = true;
		self->system->bus->Reset();
		return 0;
	}

	LUA_CLASS_FUNCTION(LuaCpu, Reset) {
		auto self = LuaCpu::FromLua(LUA, 1);
		self->system->bus->Reset();
		return 0;
	}

	LUA_CLASS_FUNCTION(LuaCpu, AttachDevice) {
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
		};

		// lame test code. this WILL be removed, I just want this for a quick test
		auto fp = std::fopen("/home/lily/test-gmod.bin", "rb");
		if(fp) {
			std::fseek(fp, 0, SEEK_END);
			auto len = std::ftell(fp);
			std::fseek(fp, 0, SEEK_SET);

			std::fread(system->ram->Raw(), 1, len, fp);
			std::fclose(fp);
		}
	}

} // namespace lcpu
