#pragma once

#include <unordered_map>

#include "GarrysMod/Lua/Interface.h"
#include "GarrysMod/Lua/LuaBase.h"
#include "GarrysMod/Lua/Types.h"
#include "LuaHelpers.hpp"

namespace lcpu::lua {

	/// A CRTP-based class which allows binding C++ to Lua, in a
	/// fairly sensible manner.
	///
	/// Classes backed by this class can have arbitrary properties
	/// created by Lua (using a backing table created by this object).
	template <class TImpl>
	struct LuaObject {
		using CFunc = GarrysMod::Lua::CFunc;
		using ILuaFunc = int (*)(GarrysMod::Lua::ILuaBase*);
		using ILuaVoidFunc = void (*)(GarrysMod::Lua::ILuaBase*);

		/// Register a C++ method.
		static void RegisterMethod(const std::string& name, CFunc func) { methods()[name] = func; }

		/// Register a getter for a value to be read.
		static void RegisterGetter(const std::string& name, ILuaVoidFunc func) { getters()[name] = func; }

		/// Register a setter. This can be used to make a
		/// C++ registered value read-write.
		/// Notes:
		///	 - Stack index 3 will always be the value to be set.
		static void RegisterSetter(const std::string& name, ILuaVoidFunc func) { setters()[name] = func; }

		virtual void AfterLuaInit() {};

		/// Create an instance of this type to give to Lua.
		/// addl. arguments are forwarded to the C++ constructor
		template <class... Args>
		static void Create(GarrysMod::Lua::ILuaBase* LUA, Args&&... args) {
			auto ptr = new TImpl(static_cast<Args&&>(args)...);
			ptr->InitLuaStuff(LUA);
			ptr->AfterLuaInit();
			LUA->PushUserType(ptr, __lua_typeid);
		}

		static TImpl* FromLua(GarrysMod::Lua::ILuaBase* LUA, int stackPos) {
			return LUA->GetUserType<TImpl>(stackPos, __lua_typeid);
		}

	   protected:
		/// This should be called first in your RegisterClass static method.
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
			LUA->Pop(); // pop metatable
			// clang-format on
		}

		/// Register a metafunction.
		/// Note that the following metafunctions are reserved by the implementation
		/// of this object, and should not be overwritten:
		///
		/// - __gc
		/// - __index
		/// - __newindex
		///
		static void RegisterMetaFunction(GarrysMod::Lua::ILuaBase* LUA, const std::string& name, CFunc func) {
			// clang-format off
			LUA->PushMetaTable(__lua_typeid);
				LUA->PushCFunction(func);
				LUA->SetField(-2, name.c_str());
			LUA->Pop();
			// clang-format on
		}

		/// A detail function used to setup some stuff in the Create() method.
		void InitLuaStuff(GarrysMod::Lua::ILuaBase* LUA) {
			lua = LUA;

			// create the table used to store user properties
			// from Lua
			LUA->CreateTable();
			tableReference = LUA->ReferenceCreate();

			// register some convinence getters
			RegisterGetter("Name", [](GarrysMod::Lua::ILuaBase* LUA) { LUA->PushString(TImpl::Name()); });
		}

		/// Get the user property table reference. This can be used
		/// to fetch user properties from native C++ code.
		int GetTableReference() { return tableReference; }

		LuaObject() = default;

		virtual ~LuaObject() {
			// free the table reference so it gets garbage collected too
			if(tableReference != -1)
				lua->ReferenceFree(tableReference);
		}

		/// The LUA interface used to create this class.
		GarrysMod::Lua::ILuaBase* lua;

	   private:
		LUA_CLASS_FUNCTION_DECL(__gc)
		LUA_CLASS_FUNCTION_DECL(__index)
		LUA_CLASS_FUNCTION_DECL(__newindex)

		/// Lua type ID for this wrapped class
		static int __lua_typeid;

		static auto& methods() {
			static std::unordered_map<std::string, CFunc> methods__;
			return methods__;
		}

		static auto& getters() {
			static std::unordered_map<std::string, ILuaVoidFunc> getters__;
			return getters__;
		}

		static auto& setters() {
			static std::unordered_map<std::string, ILuaVoidFunc> setters__;
			return setters__;
		}

		// instance stuff
		int tableReference { -1 };
	};

	template <class TImpl>
	int LuaObject<TImpl>::__lua_typeid = 0;

	template <class TImpl>
	LUA_CLASS_FUNCTION(LuaObject<TImpl>, __gc) {
		auto self = FromLua(LUA, 1);
		if(self != nullptr) {
			lucore::LogDebug("GCing LuaObject {} @ {:p}", TImpl::Name(), static_cast<void*>(self));
			delete self;
		}
		return 0;
	}

	template <class TImpl>
	LUA_CLASS_FUNCTION(LuaObject<TImpl>, __index) {
		auto self = FromLua(LUA, 1);

		// If the key is something we support,
		if(LUA->GetType(2) == GarrysMod::Lua::Type::String) {
			auto& methods = LuaObject::methods();
			auto& getters = LuaObject::getters();
			auto key = GetLuaString(LUA, 2);

			if(methods.find(key) != methods.end()) {
				LUA->PushCFunction(methods[key]);
				return 1;
			}

			if(getters.find(key) != getters.end()) {
				// getters explicitly push their return onto the stack
				getters[key](LUA);
				return 1;
			}

			lucore::LogDebug("LuaObject::__index({}) going to table", key);
		}

		// Failing to look up an item is not fatal;
		// we simply then look up the key in the backing table.
		// clang-format off
		LUA->ReferencePush(self->tableReference); // push table reference
			LUA->Push(2); 				 // push key onto the stack
		LUA->GetTable(-2); 				 // push table[key] onto the top of stack
		// clang-format on
		return 1;
	}

	template <class TImpl>
	LUA_CLASS_FUNCTION(LuaObject<TImpl>, __newindex) {
		auto self = FromLua(LUA, 1);

		if(LUA->GetType(2) == GarrysMod::Lua::Type::String) {
			auto& methods = LuaObject::methods();
			auto& getters = LuaObject::getters();
			auto& setters = LuaObject::setters();
			auto key = GetLuaString(LUA, 2);

			// don't allow overwriting methods
			if(methods.find(key) != methods.end())
				return 0;

			// or read-only C++ values
			if(getters.find(key) != getters.end() && setters.find(key) == setters.end())
				return 0;

			if(setters.find(key) != setters.end()) {
				setters[key](LUA);
				return 0;
			}


			lucore::LogDebug("LuaObject::__newindex({}) going to table", key);
		}


		// set the provided value onto the table
		// clang-format off
		LUA->ReferencePush(self->tableReference); // table
			LUA->Push(2);				 // key
			LUA->Push(3);				 // value
			LUA->SetTable(-3);			 // table[key] = value
		LUA->Pop();
		// clang-format on
		return 0;
	}

} // namespace lcpu::lua
