#include "LuaCpu.hpp"

#include <lucore/Logger.hpp>

#include "LuaMember.hpp"

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
			fputc(c, stderr);
		}
	}
};

int LuaCpu::__lua_typeid;

void LuaCpu::Bind(GarrysMod::Lua::ILuaBase* LUA) {
	lucore::LogInfo("In LuaCpu::Bind()");
	__lua_typeid = LUA->CreateMetaTable("LuaCpu");
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG);
		LUA->PushNumber(__lua_typeid);
		LUA->SetField(-2, "LuaCpu__typeid");
	LUA->Pop(); // Pop the registry

		LUA->Push(-1);
		// This method is called when the GC is done with our stuff
		LUA->PushCFunction(__gc);
		LUA->SetField(-1, "__gc");

		LUA->PushCFunction(Cycle);
		LUA->SetField(-1, "Cycle");
	LUA->Pop();
}

void LuaCpu::Create(GarrysMod::Lua::ILuaBase* LUA, u32 memorySize) {
	LUA->PushUserType(new LuaCpu(memorySize), __lua_typeid);
}

LuaCpu::LuaCpu(u32 memorySize) {
	system = riscv::System::Create(memorySize);
	system->OnPowerOff = [&]() { this->OnSysconShutdown(); };

	system->bus->AttachDevice(new SimpleUartDevice);

	// lame test code. this WILL be removed, I just want this for a quick test
	auto fp = std::fopen("/home/lily/gs/gmod/garrysmod/addons/lcpu/native/projects/riscv_test_harness/test/test.bin", "rb");
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

void LuaCpu::CycleImpl() {
	if(!poweredOn)
		return;

	system->Step();
}

void LuaCpu::OnSysconShutdown() {
	poweredOn = false;
}

LUA_MEMBER_FUNCTION(LuaCpu, __gc) {
	auto self = LUA->GetUserType<LuaCpu>(1, __lua_typeid);
	if(self != nullptr) { // GetUserType returns nullptr on failure
		delete self;
	}

	return 0;
}

LUA_MEMBER_FUNCTION(LuaCpu, Cycle) {
	auto self = LUA->GetUserType<LuaCpu>(1, __lua_typeid);
	if(!self) {
		LUA->ThrowError("invalid self argument for LuaCpu:Cycle()");
	}

	self->CycleImpl();
	return 0;
}
