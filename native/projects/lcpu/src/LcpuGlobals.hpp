#include "LuaHelpers.hpp"

namespace lcpu {
/// This should be bumped on any incompatible change to the native bindings
/// that would break older Lua code, or requires newer Lua code to run.
#define LCPU_MODULE_VERSION 1

	void GlobalsBind(GarrysMod::Lua::ILuaBase* LUA);

} // namespace lcpu
