#pragma once

#include <string_view>

namespace lucore {

	struct Library {
		using Handle = void*;

		/// Open an already loaded library
		static Library* OpenExisting(std::string_view dllname);

		/// Query if [dllname] is loaded in the process.
		static bool Loaded(std::string_view dllname);

		~Library();

		template <class T>
		T Symbol(std::string_view symbolName) {
			return reinterpret_cast<T>(SymbolImpl(symbolName.data()));
		}

	   private:
		void* SymbolImpl(const char* symbol);
		constexpr explicit Library(Handle handle) : handle(handle) {}
		Handle handle {};
	};

} // namespace lucore
