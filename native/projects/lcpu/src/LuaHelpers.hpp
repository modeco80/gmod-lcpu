//! Helpers for lua binding
#pragma once

#include <GarrysMod/Lua/Interface.h>

#include <lucore/Logger.hpp>
#include <unordered_map>

#include "GarrysMod/Lua/LuaBase.h"

// These are like the official GMOD header LUA_FUNCTION but allow forward declaration
// and implementation inside of classes, making writing class bindings that much less
// of a PITA. Nifty!
#define LUA_MEMBER_FUNCTION(FUNC)                                \
	static int FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA); \
	static int FUNC(lua_State* L) {                              \
		GarrysMod::Lua::ILuaBase* LUA = L->luabase;              \
		LUA->SetState(L);                                        \
		return FUNC##__ImpStatic(LUA);                           \
	}

#define LUA_MEMBER_FUNCTION_IMPLEMENT(CLASS, FUNC) int CLASS::FUNC##__ImpStatic(GarrysMod::Lua::ILuaBase* LUA)

// this synthesizes a lambda which takes the stack argument to get. this can actually also be
// stored as a variable for later usage (... if you so desire?)
#define LUA_CLASS_GET(T)                                       \
	[LUA](int stackPos) {                                      \
		LUA->CheckType(stackPos, T::__lua_typeid);             \
		return LUA->GetUserType<T>(stackPos, T::__lua_typeid); \
	}

// This class binding package always implements the __gc metamethod
// to free any C++ object bound to Lua.

// Declare required binding variables.
#define LUA_CLASS_BIND_VARIABLES(ACCESS_LEVEL) \
   public:                                     \
	static int __lua_typeid;                   \
	ACCESS_LEVEL:                              \
	LUA_MEMBER_FUNCTION(__gc);

// Implement required binding variables (typically in a .cpp file).
#define LUA_CLASS_BIND_VARIABLES_IMPLEMENT(T)                                        \
	int T::__lua_typeid = 0;                                                         \
	LUA_MEMBER_FUNCTION_IMPLEMENT(T, __gc) {                                         \
		LUA->CheckType(1, T::__lua_typeid);                                          \
		auto self = LUA->GetUserType<T>(1, T::__lua_typeid);                         \
		if(self != nullptr) { /* GetUserType returns nullptr on failure */           \
			lucore::LogInfo("GCing {} object @ {:p}", #T, static_cast<void*>(self)); \
			delete self;                                                             \
		}                                                                            \
		return 0;                                                                    \
	}

// Begin the Bind() method of a class. This just sets up boilerplate
// and required things to setup a class.
#define LUA_CLASS_BIND_BEGIN(T)                    \
	T::__lua_typeid = LUA->CreateMetaTable(#T);    \
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG); \
	LUA->PushNumber(T::__lua_typeid);              \
	LUA->SetField(-2, #T "__typeid");              \
	LUA->Pop(); /* pop registry */                 \
	LUA->Push(-1);                                 \
	LUA->SetField(-2, "__index");                  \
	LUA_SET_C_FUNCTION(__gc)

// End the Bind() method.
#define LUA_CLASS_BIND_END() LUA->Pop();

// Set a C function as a field.
#define LUA_SET_C_FUNCTION(name) \
	LUA->PushCFunction(name);    \
	LUA->SetField(-2, #name);

// Set a C function as a field with an alternative field name.
#define LUA_SET_C_FUNCTION_NAME(name, altName) \
	LUA->PushCFunction(name);                  \
	LUA->SetField(-2, altName);

namespace lcpu::lua {

	inline std::string GetLuaString(GarrysMod::Lua::ILuaBase* LUA, int stackPos) {
		unsigned len {};
		auto ptr = LUA->GetString(stackPos, &len);
		if(ptr) {
			return std::string(ptr, len);
		} else {
			return {};
		}
	}

	/// A CRTP-based class which allows binding C++ to Lua, in a
	/// fairly sensible manner.
	template <class TImpl>
	struct LuaObject {
		using CFunc = GarrysMod::Lua::CFunc;
		using ILuaFunc = void (*)(GarrysMod::Lua::ILuaBase*);

		/// Register a C++ method.
		static void RegisterMethod(const std::string& name, CFunc func) { methods()[name] = func; }

		/// Register a getter for a value to be read.
		static void RegisterGetter(const std::string& name, ILuaFunc func) { getters()[name] = func; }

		/// Register a setter. This can be used to make a
		/// C++ registered value read-write.
		static void RegisterSetter(const std::string& name, ILuaFunc func) { setters()[name] = func; }

		// addl. arguments are forwarded to the C++ constructor
		template <class... Args>
		static void Create(GarrysMod::Lua::ILuaBase* LUA, Args&&... args) {
			auto ptr = new TImpl(static_cast<Args&&>(args)...);
			ptr->InitLuaStuff(LUA);
			LUA->PushUserType(ptr, __lua_typeid);
		}

		static TImpl* FromLua(GarrysMod::Lua::ILuaBase* LUA, int stackPos) {
			LUA->CheckType(stackPos, __lua_typeid);
			return LUA->GetUserType<TImpl>(stackPos, __lua_typeid);
		}

	   protected:
		/// This should be called first in your RegisterClass static method.
		/// This doesn't pop the metatable off so you can keep adding things to it
		static void RegisterClassStart(GarrysMod::Lua::ILuaBase* LUA) {
			auto typeid_name = std::format("{}__typeid", TImpl::Name());

			// clang-format off
			__lua_typeid = LUA->CreateMetaTable(TImpl::Name());
				LUA->PushSpecial(GarrysMod::Lua::SPECIAL_REG);
					LUA->PushNumber(__lua_typeid);
					LUA->SetField(-2, typeid_name.c_str());
				LUA->Pop(); // pop registry
				// add in required metamethods
				LUA_SET_C_FUNCTION(__gc)
				LUA_SET_C_FUNCTION(__index)
				LUA_SET_C_FUNCTION(__newindex)
			// clang-format on
		}

		/// Call in your RegisterClass() method when done registering metamethods.
		static void RegisterClassEnd(GarrysMod::Lua::ILuaBase* LUA) { LUA->Pop(); }

		/// A detail function used to setup some stuff in the Create() method.
		void InitLuaStuff(GarrysMod::Lua::ILuaBase* LUA) {
			lua = LUA;

			// create the table used to store user properties
			// from Lua
			LUA->CreateTable();
			tableReference = LUA->ReferenceCreate();

			// register some convinence things
			RegisterGetter("Name", [](GarrysMod::Lua::ILuaBase* LUA) { LUA->PushString(TImpl::Name()); });
		}

		LuaObject() = default;
		~LuaObject() {
			// free the table reference
			if(tableReference != -1)
				lua->ReferenceFree(tableReference);
		}


		/// The LUA interface used to create this class.
		GarrysMod::Lua::ILuaBase* lua;

	   private:
		// base metamethods
		LUA_MEMBER_FUNCTION(__gc)
		LUA_MEMBER_FUNCTION(__index)
		LUA_MEMBER_FUNCTION(__newindex)

		// static stuff
		static int __lua_typeid;

		static auto& methods() {
			static std::unordered_map<std::string, GarrysMod::Lua::CFunc> methods__;
			return methods__;
		}

		static auto& getters() {
			static std::unordered_map<std::string, ILuaFunc> getters__;
			return getters__;
		}

		static auto& setters() {
			static std::unordered_map<std::string, ILuaFunc> setters__;
			return setters__;
		}

		// instance stuff
		int tableReference { -1 };
	};

	template <class TImpl>
	int LuaObject<TImpl>::__lua_typeid = 0;

	template <class TImpl>
	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaObject<TImpl>, __gc) {
		auto self = FromLua(LUA, 1);
		if(self != nullptr) {
			lucore::LogInfo("GCing LuaObject-based object @ {:p}", static_cast<void*>(self));
			delete self;
		}
		return 0;
	}

	template <class TImpl>
	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaObject<TImpl>, __index) {
		auto self = FromLua(LUA, 1);

		if(LUA->GetType(2) == GarrysMod::Lua::Type::String) {
			auto key = GetLuaString(LUA, 2);

			if(methods().find(key) != methods().end()) {
				LUA->PushCFunction(methods()[key]);
				return 1;
			}

			if(getters().find(key) != getters().end()) {
				// getters explicitly push their return onto the stack
				getters()[key](LUA);
				return 1;
			}
		}

		// look up from the table
		// clang-format off
		LUA->ReferencePush(self->tableReference);
			LUA->Push(2);
		LUA->GetTable(-2);
		// clang-format on
		return 1;
	}

	template <class TImpl>
	LUA_MEMBER_FUNCTION_IMPLEMENT(LuaObject<TImpl>, __newindex) {
		auto self = FromLua(LUA, 1);

		if(LUA->GetType(2) == GarrysMod::Lua::Type::String) {
			auto key = GetLuaString(LUA, 2);

			// don't allow overwriting methods
			if(methods().find(key) != methods().end())
				return 0;

			// or read-only values
			if(getters().find(key) != getters().end() && setters().find(key) == setters().end())
				return 0;

			if(setters().find(key) != setters().end()) {
				// for ergonomic sake only I kind of want to make this like
				// LUA->Push(3) so that -1 (top of stack) is the value
				// a bit cleaner. idk
				setters()[key](LUA);
				return 0;
			}
		}

		// push onto the table
		// clang-format off
		LUA->ReferencePush(self->tableReference);
			LUA->Push(2);
			LUA->Push(3);
			LUA->SetTable(-3);
		LUA->Pop();
		// clang-format on
		return 0;
	}

} // namespace lcpu::lua
