#include "OsLibrary.hpp"

#include <dlfcn.h>

namespace lucore::detail {

	OsLibraryHandle OsOpenLibrary(const char* filename) {
		return dlopen(filename, RTLD_LAZY);
	}

	OsLibraryHandle OsOpenExistingLibrary(const char* filename) {
		return dlopen(filename, RTLD_LAZY);
	}

	bool OsLibraryLoaded(const char* filename) {
		// RTLD_NOLOAD tells the dynamic linker *not* to load
		// the module if it's not loaded in this process, which
		// allows us to test for if a module is loaded or not
		return dlopen(filename, RTLD_NOLOAD | RTLD_LAZY) != nullptr;
	}

	void* OsLibrarySymbol(OsLibraryHandle dll, const char* symbolName) {
		return dlsym(dll, symbolName);
	}

	void OsFreeLibrary(OsLibraryHandle handle) {
		// The reference count on *Nix *will* be incremented by dlopen(),
		// therefore we do have to always free libraries.
		dlclose(handle);
	}
} // namespace lucore::detail
