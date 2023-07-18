#include <format>
#include <lucore/Library.hpp>
#include <string>

#include "OsLibrary.hpp"

namespace lucore {

	namespace {
		std::string FormatLibraryName(std::string_view dllName) {
#ifdef _WIN32
			return std::format("{}.dll", dllName);
#else
			return std::format("lib{}.so", dllName);
#endif
		}
	} // namespace

	Library* Library::Open(std::string_view dllname) {
		return new Library(
		static_cast<void*>(detail::OsOpenLibrary(FormatLibraryName(dllname).c_str())));
	}

	bool Library::Loaded(std::string_view dllname) {
		return detail::OsLibraryLoaded(FormatLibraryName(dllname).c_str());
	}

	Library::~Library() {
		if(handle) {
			detail::OsFreeLibrary(static_cast<detail::OsLibraryHandle>(handle));
		}
	}

	void* Library::SymbolImpl(const char* symbolName) {
		return detail::OsLibrarySymbol(static_cast<detail::OsLibraryHandle>(handle), symbolName);
	}
} // namespace lucore
