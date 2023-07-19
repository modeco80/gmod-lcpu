#include "OsLibrary.hpp"

#include <dlfcn.h>

namespace lucore::detail {
	OsLibraryHandle OsOpenLibrary(const char* filename) {
		return dlopen(filename, RTLD_LAZY);
	}

	bool OsLibraryLoaded(const char* filename) {
		return dlopen(filename, RTLD_NOLOAD | RTLD_LAZY) != nullptr;
	}

	void* OsLibrarySymbol(OsLibraryHandle dll, const char* symbolName) {
		return dlsym(dll, symbolName);
	}

	void OsFreeLibrary(OsLibraryHandle handle) {
		// The reference count on *Nix will be incremented by the launcher
		// process itself, therefore we do not risk accidentally pulling the
		// library out of the rug of the engine in either case.
		dlclose(handle);
	}
} // namespace lucore::detail
