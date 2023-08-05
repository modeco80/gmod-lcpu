#include "OsLibrary.hpp"

#define _WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace lucore::detail {

	OsLibraryHandle OsOpenLibrary(const char* filename) {
		return reinterpret_cast<OsLibraryHandle>(LoadLibraryA(filename));
	}

	OsLibraryHandle OsOpenExistingLibrary(const char* filename) {
		if(!OsLibraryLoaded(filename))
			return nullptr;

		return reinterpret_cast<OsLibraryHandle>(GetModuleHandleA(filename));
	}

	bool OsLibraryLoaded(const char* filename) {
		return GetModuleHandleA(filename) != nullptr;
	}

	void* OsLibrarySymbol(OsLibraryHandle dll, const char* symbolName) {
		return GetProcAddressA(reinterpret_cast<HMODULE>(dll), symbolName);
	}

	void OsFreeLibrary(OsLibraryHandle handle) {
		// Note that this function should never be called on a handle retrieved
		// from OsOpenExistingLibrary(); GetModuleHandle() does **not** increment
		// the module's reference count and therefore there is a real risk of accidentally
		// freeing the module and causing crashes 
		FreeLibraryA(reinterpret_cast<HMODULE>(handle));
	}

} // namespace lucore::detail
