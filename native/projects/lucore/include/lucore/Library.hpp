#pragma once

#include <lucore/Types.hpp>
#include <string_view>

namespace lucore {

	/// A platform-independent class for working with a shared library
	/// as a resource.
	struct Library {
		using Handle = void*;

		/// Open a library regardless of if it's been loaded or not.
		static Unique<Library> Open(std::string_view name);

		/// Open an already loaded library
		static Unique<Library> OpenExisting(std::string_view dllname);

		/// Query if [dllname] is loaded in the process.
		static bool Loaded(std::string_view dllname);

		~Library();

		template <class T>
		T Symbol(std::string_view symbolName) {
			return reinterpret_cast<T>(SymbolImpl(symbolName.data()));
		}

	   private:
	   	struct ExistingTag{};
		constexpr static inline ExistingTag Existing{};
		
		constexpr explicit Library(Handle handle) : handle(handle) {}
		constexpr explicit Library(Handle handle, ExistingTag) : handle(handle), existing(true) {}

		void* SymbolImpl(const char* symbol);
		Handle handle {};

		/// Tracks if this Library instance was created using [Library::OpenExisting].
		bool existing{false};
	};

} // namespace lucore
