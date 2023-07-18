//! Operating-system independent utilities for opening
//! shared libraries. This is currently a detail-only
//! Lucore API, and its stability is NOT guaranteed.

namespace lucore::detail {
	/// Opaque handle type for libraries.
	using OsLibraryHandle = void*;

	/// Open a library.
	OsLibraryHandle OsOpenLibrary(const char* filename);

	/// Query if the library with the given [filename] is loaded.
	bool OsLibraryLoaded(const char* filename);

	/// Get a pointer to the symbol of a library.
	void* OsLibrarySymbol(OsLibraryHandle dll, const char* symbolName);

	/// Free the library.
	void OsFreeLibrary(OsLibraryHandle handle);

} // namespace lucore::detail
