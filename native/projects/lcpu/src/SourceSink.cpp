#include "SourceSink.hpp"

#include <lucore/Library.hpp>
#include <lucore/Types.hpp>

// The old non-beta branch of GMod on Linux has multiple tier0 libraries for client and server.
// This compatibility define allows to support that case (for now).
#define LCPU_SUPPORT_OLD_GMOD

namespace tier0 {
	// TODO: For now this'll leak..
	lucore::Library* library = nullptr;

	using Msg_t = void (*)(const char*, ...);
	Msg_t Msg {};

} // namespace tier0

namespace lcpu {

	SourceSink& SourceSink::The() {
		static SourceSink sink;
		return sink;
	}

	SourceSink::SourceSink() {
#ifdef LCPU_SUPPORT_OLD_GMOD
		constexpr static std::string_view tier0_libraries[] {
#ifdef __linux__
			"tier0_srv"
#endif
			"tier0"
		};

		for(auto lib : tier0_libraries) {
			if(lucore::Library::Loaded(lib)) {
				// Found the correct tier0 library to open; use that.
				tier0::library = lucore::Library::Open(lib);
				break;
			}
		}
#else
		// The x86-64 branch of GMod, including the 32-bit binaries in the branch,
		// have a single tier0 library, which makes the codepath much simpler.
		tier0::library = lucore::Library::Open("tier0");
#endif

#define GRAB_SYMBOL(name, T) tier0::name = tier0::library->Symbol<T>(#name);
		GRAB_SYMBOL(Msg, tier0::Msg_t);
	}

	SourceSink::~SourceSink() {
		delete tier0::library;
	}

	void SourceSink::OutputMessage(const lucore::Logger::MessageData& data) {
		auto formatted =
		std::format("[LCPU Native/{}] [{}] {}", lucore::Logger::SeverityToString(data.severity),
					data.time, std::vformat(data.format, data.args));

		tier0::Msg("%s\n", formatted.c_str());
	}

} // namespace lcpu
