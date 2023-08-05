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

	Unique<Library> Library::Open(std::string_view dllname) {
		auto name = FormatLibraryName(dllname);
		auto handle = detail::OsOpenLibrary(name.c_str());

		if(!handle) {
#ifndef _WIN32
			// Try without a `lib` prefix next. If this fails, give up.
			name = std::format("{}.so", dllname);
			handle = detail::OsOpenLibrary(name.c_str());
			if(!handle)
				return nullptr;
#else
			return nullptr;
#endif
		}

		return Unique<Library>(new Library(handle));
	}

	Unique<Library> Library::OpenExisting(std::string_view dllname) {
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

		return Unique<Library>(new Library(detail::OsOpenExistingLibrary(name.c_str()), Existing));
	}

	bool Library::Loaded(std::string_view dllname) {
		return detail::OsLibraryLoaded(FormatLibraryName(dllname).c_str());
	}

	Library::~Library() {
		if(handle) {
			// See OsLibrary.win32.cpp for reasoning of this weird ifdef thing
#ifdef _WIN32
			if(!existing)
#endif
				detail::OsFreeLibrary(handle);
		}
	}

	void* Library::SymbolImpl(const char* symbolName) {
		return detail::OsLibrarySymbol(handle, symbolName);
	}
} // namespace lucore
