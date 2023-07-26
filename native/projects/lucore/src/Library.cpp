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

	Library* Library::OpenExisting(std::string_view dllname) {
		auto name = FormatLibraryName(dllname);
		if(!detail::OsLibraryLoaded(name.c_str())) {
#ifndef _WIN32
			// Try without a prefix; some libraries need this.
			name = std::format("{}.so", dllname);
			if(!detail::OsLibraryLoaded(name.c_str()))
				return nullptr;
#else
			return nullptr;
#endif
		}

		return new Library(detail::OsOpenLibrary(name.c_str()));
	}

	bool Library::Loaded(std::string_view dllname) {
		return detail::OsLibraryLoaded(FormatLibraryName(dllname).c_str());
	}

	Library::~Library() {
		if(handle) {
			detail::OsFreeLibrary(handle);
		}
	}

	void* Library::SymbolImpl(const char* symbolName) {
		return detail::OsLibrarySymbol(handle, symbolName);
	}
} // namespace lucore
