#include "LuaDevice.hpp"

namespace lcpu {
	void LuaDevice::RegisterClass(GarrysMod::Lua::ILuaBase* LUA) {
		RegisterClassStart(LUA);

		RegisterGetter("Base", [](GarrysMod::Lua::ILuaBase* LUA) {
			auto self = LuaDevice::FromLua(LUA, 1);
			LUA->PushNumber(static_cast<double>(self->base));
		});

		RegisterGetter("Size", [](GarrysMod::Lua::ILuaBase* LUA) {
			auto self = LuaDevice::FromLua(LUA, 1);
			LUA->PushNumber(static_cast<double>(self->size));
		});
	}

	bool LuaDevice::Clocked() const {
		// no real good rationale for checking here,
		// since function calling does bail out properly
		return true;
	}

	void LuaDevice::Clock() {
		// clang-format off
		lua->ReferencePush(GetTableReference());
			lua->GetField(-1,"Clock");
				if(lua->GetType(-1) == GarrysMod::Lua::Type::Function) { 
					lua->Push(-2); // 'self' argument
					lua->Call(1, 0);
				} else {
					lua->Pop(); // pop the Clock function off the stack
				}
		lua->Pop(); // pop the reference
		// clang-format on
	}

	riscv::Address LuaDevice::Base() const {
		return base;
	}

	riscv::Address LuaDevice::Size() const {
		return base;
	}

	u32 LuaDevice::Peek(riscv::Address address) {
		// clang-format off
		lua->ReferencePush(GetTableReference());
			lua->GetField(-1,"Peek");
				if(lua->GetType(-1) == GarrysMod::Lua::Type::Function) { 
					lua->Push(-2); // 'self' argument
					lua->PushNumber(static_cast<double>(address));
					lua->Call(2, 1);

					auto result = static_cast<u32>(lua->GetNumber(-1));
					lua->Pop(2); // pop result and the table off
					return result;
				} else {
					lua->Pop(); // pop whatever Peek is off the stack
				}
		lua->Pop(); // pop the table reference
		// clang-format on
		return 0xffffffff;
	}

	void LuaDevice::Poke(riscv::Address address, u32 value) {
		// clang-format off
		lua->ReferencePush(GetTableReference());
			lua->GetField(-1,"Poke");
				if(lua->GetType(-1) == GarrysMod::Lua::Type::Function) { 
					lua->Push(-2); // 'self' argument
					lua->PushNumber(static_cast<double>(address));
					lua->PushNumber(static_cast<double>(value));
					lua->Call(3, 0);
				} else {
					lua->Pop(); // pop whatever Peek is
				}
		lua->Pop(); // pop the table reference
		// clang-format on
	}

	void LuaDevice::Reset() {
		// clang-format off
		lua->ReferencePush(GetTableReference());
			lua->GetField(-1,"Reset");
			if(lua->GetType(-1) == GarrysMod::Lua::Type::Function) { 
				lua->Push(-2); // 'self' argument
				lua->Call(1, 0);
			} else {
				lua->Pop(); // pop whatever reset is
			}
		lua->Pop(); // pop the reference
		// clang-format on
	}

	LuaDevice::LuaDevice(riscv::Address base, riscv::Address size) : base(base), size(size) {
	}

	void LuaDevice::AfterLuaInit() {
		// Our Lua callbacks only get the table, not the actual userdata,
		// ao we have to mirror things :( kinda sucks, but meh
		
		// clang-format off
		lua->ReferencePush(GetTableReference());
			lua->PushNumber(static_cast<double>(base));
			lua->SetField(-2, "Base");
			lua->PushNumber(static_cast<double>(base));
			lua->SetField(-2, "Size");
		lua->Pop();
		// clang-format on
	}
} // namespace lcpu
