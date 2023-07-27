#include "LuaDevice.hpp"

LUA_CLASS_BIND_VARIABLES_IMPLEMENT(LuaDevice);

bool LuaDevice::Clocked() const {
	// no real good rationale for checking here,
	// since function calling does bail out properly
	return true;
}

void LuaDevice::Clock() {
	LuaState->ReferencePush(tableReference);
		LuaState->GetField(-1,"Clock");
			if(LuaState->GetType(-1) == GarrysMod::Lua::Type::Function) { 
				LuaState->Push(-2); // 'self' argument
				LuaState->Call(1, 0);
			} else {
				LuaState->Pop(); // pop the Clock function off the stack
			}
	LuaState->Pop(); // pop the reference
}

riscv::Address LuaDevice::Base() const {
	return base;
}

riscv::Address LuaDevice::Size() const {
	return base;
}

u32 LuaDevice::Peek(riscv::Address address) {
	/*if(peekHandlerReference != -1) {
		LuaState->ReferencePush(resetHandlerReference);
		LuaState->PushNumber(static_cast<double>(address));
		LuaState->Call(1, 1);
		auto result = LuaState->GetNumber(-1);
		LuaState->Pop();
		return static_cast<u32>(result);
	}*/

	LuaState->ReferencePush(tableReference);
		LuaState->GetField(-1,"Peek");
			if(LuaState->GetType(-1) == GarrysMod::Lua::Type::Function) { 
				LuaState->Push(-2); // 'self' argument
				LuaState->PushNumber(static_cast<double>(address));
				LuaState->Call(2, 1);

				auto result = static_cast<u32>(LuaState->GetNumber(-1));
				LuaState->Pop(2); // pop result and the table off
				return result;
			} else {
				LuaState->Pop(); // pop whatever Peek is off the stack
			}
	LuaState->Pop(); // pop the table reference
	return 0xffffffff;
}

void LuaDevice::Poke(riscv::Address address, u32 value) {
	/*if(pokeHandlerReference != -1) {
		LuaState->ReferencePush(pokeHandlerReference);
		LuaState->PushNumber(address);
		LuaState->PushNumber(value);
		LuaState->Call(2, 0);
	}*/

	LuaState->ReferencePush(tableReference);
		LuaState->GetField(-1,"Poke");
			if(LuaState->GetType(-1) == GarrysMod::Lua::Type::Function) { 
				LuaState->Push(-2); // 'self' argument
				LuaState->PushNumber(static_cast<double>(address));
				LuaState->PushNumber(static_cast<double>(value));
				LuaState->Call(3, 0);
			} else {
				LuaState->Pop(); // pop whatever Peek is
			}
	LuaState->Pop(); // pop the table reference
}

void LuaDevice::Reset() {
	/*if(resetHandlerReference != -1) {
		LuaState->ReferencePush(resetHandlerReference);
		LuaState->Call(0, 0);
	}*/

	LuaState->ReferencePush(tableReference);
		LuaState->GetField(-1,"Reset");
		if(LuaState->GetType(-1) == GarrysMod::Lua::Type::Function) { 
			LuaState->Push(-2); // 'self' argument
			LuaState->Call(1, 0);
		} else {
			LuaState->Pop(); // pop whatever reset is
		}
	LuaState->Pop(); // pop the reference
}

LuaDevice::LuaDevice(riscv::Address base, riscv::Address size) : base(base), size(size) {
}

LuaDevice::~LuaDevice() {
	// Free all refererences
	if(tableReference == -1)
		LuaState->ReferenceFree(tableReference);
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaDevice, __index) {
	auto self = LUA_CLASS_GET(LuaDevice)(1);
	//lucore::LogInfo("metamethod __index call");
	
	// TODO: before moving this to a shared lua object class thing
	// and moving the CPU class to use this way of doing things
	// I should probably try and like, add stuff to ensure native
	// methods can be registered as well,,
	LUA->ReferencePush(self->tableReference);
	LUA->Push(2);
	LUA->GetTable(-2);
	return 1; // the value
}

LUA_MEMBER_FUNCTION_IMPLEMENT(LuaDevice, __newindex) {
	auto self = LUA_CLASS_GET(LuaDevice)(1);
	//lucore::LogInfo("metamethod __newindex call");

	// Always push onto the table. 
	// TODO: This function
	// should error on attempt to __newindex any native methods
	// (when moved to a shared place)
	LUA->ReferencePush(self->tableReference);
	LUA->Push(2);
	LUA->Push(3);
	LUA->SetTable(-3);
	LUA->Pop();
	return 0;
}

void LuaDevice::Bind(GarrysMod::Lua::ILuaBase* LUA) {
	// clang-format off
	//LUA_CLASS_BIND_BEGIN(LuaDevice)
	__lua_typeid = LUA->CreateMetaTable("LuaDevice");                                
		LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG);                             
			LUA->PushNumber(__lua_typeid);                                          
			LUA->SetField(-2, "LuaDevice__typeid");                                          
		LUA->Pop(); /* pop registry */                                             

		LUA_SET_C_FUNCTION(__gc)
		LUA_SET_C_FUNCTION(__index)
		LUA_SET_C_FUNCTION(__newindex)
	LUA_CLASS_BIND_END();
	// clang-format on


}

void LuaDevice::Create(GarrysMod::Lua::ILuaBase* LUA, riscv::Address base, riscv::Address size) {
	auto device = new LuaDevice(base, size);
	device->LuaState = LUA;

	LUA->CreateTable();
		device->tableReference = LUA->ReferenceCreate();
	LUA->Pop();

	// push base/size properties for lua to have a looksee at !
	// ideally these should be handled as metamethods in __index,
	// but i don't quite feel like making gmod sol2 yet /shrug
	LUA->ReferencePush(device->tableReference);
		LUA->PushNumber(static_cast<double>(base));
		LUA->SetField(-2, "Base");
		LUA->PushNumber(static_cast<double>(base));
		LUA->SetField(-2, "Size");
	LUA->Pop();

	LUA->PushUserType(device, __lua_typeid);
}
