#include "OsLibrary.hpp"

#define _WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace lucore::detail {

	OsLibraryHandle OsOpenLibrary(const char* filename) {
		return reinterpret_cast<OsLibraryHandle>(GetModuleHandleA(filename);
	}

	bool OsLibraryLoaded(const char* filename) {
		return GetModuleHandleA(filename) != nullptr;
	}

	void* OsLibrarySymbol(OsLibraryHandle dll, const char* symbolName) {
		return GetProcAddressA(reinterpret_cast<HMODULE*>(dll), symbolName);
	}

	void OsFreeLibrary(OsLibraryHandle handle) {
		// GetModuleHandle*() does not increment the reference count;
		// therefore, we have nothing to do here on Windows.
	}

} // namespace lucore::detail
