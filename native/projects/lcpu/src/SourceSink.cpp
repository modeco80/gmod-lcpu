#include "SourceSink.hpp"

#include <cstdlib>
#include <lucore/Assert.hpp>
#include <lucore/Library.hpp>
#include <lucore/Types.hpp>

// The old non-beta branch of GMod on Linux has multiple tier0 libraries for client and server.
// This compatibility define allows to support that case (for now). Once this define is removed,
// the old codepath can be totally removed.
#define LCPU_SUPPORT_OLD_GMOD

namespace tier0 {
	lucore::Library* library = nullptr;

	using Msg_t = void (*)(const char*, ...);
	Msg_t Msg {};

	bool OpenLibrary() {
#ifdef LCPU_SUPPORT_OLD_GMOD
		constexpr static std::string_view tier0_libraries[] {
	#ifdef __linux__
			"tier0_srv",
	#endif
			"tier0"
		};

		for(auto lib : tier0_libraries) {
			if(lucore::Library::Loaded(lib)) {
				// Found the correct tier0 library to open; use that.
				tier0::library = lucore::Library::OpenExisting(lib);
				break;
			}
		}
#else
		// The x86-64 branch of GMod, including the 32-bit binaries in the branch,
		// have a single tier0 library, which makes the codepath much simpler.
		// Hopefully I can switch to this path at some point.
		tier0::library = lucore::Library::OpenExisting("tier0");
#endif

		if(tier0::library == nullptr)
			return false;
		return true;
	}

	bool GrabSymbols() {
#define GRAB_SYMBOL(name, T)          \
	name = library->Symbol<T>(#name); \
	if(name == nullptr)               \
		return false;
		GRAB_SYMBOL(Msg, Msg_t);
#undef GRAB_SYMBOL
		return true;
	}

} // namespace tier0

namespace lcpu {

	SourceSink& SourceSink::The() {
		static SourceSink sink;
		return sink;
	}

	SourceSink::SourceSink() {
		if(!tier0::OpenLibrary()) {
			std::printf("Tier0 could not be opened\n");
			std::quick_exit(10);
		}

		// TODO: A bit nicer of an error message?
		// Explain *what* to do if you see this message.
		if(!tier0::GrabSymbols()) {
			std::printf("Tier0 symbols could not be grabbed\n");
			std::quick_exit(10);
		}
	}

	SourceSink::~SourceSink() {
		delete tier0::library;
	}

	void SourceSink::OutputMessage(const lucore::Logger::MessageData& data) {
		auto formatted =
		std::format("[LCPU/{}] [{}] {}", lucore::Logger::SeverityToString(data.severity), data.time, std::vformat(data.format, data.args));

		tier0::Msg("%s\n", formatted.c_str());
	}

} // namespace lcpu
