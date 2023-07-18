#include "SourceSink.hpp"

#include <lucore/Library.hpp>

namespace tier0 {
	// TODO: For now this'll leak..
	lucore::Library* library = nullptr;

	// TODO: Switch this to using the LoggingSystem. It is available on the x86-64 branch of
	// gmod.
	using Msg_t = void (*)(const char* format, ...);
	Msg_t Msg = nullptr;
	Msg_t Warning = nullptr;
} // namespace tier0

namespace lcpu {

	SourceSink& SourceSink::The() {
		static SourceSink sink;
		return sink;
	}

	void SourceSink::Init() {
		// Modern 64-bit GMod has one tier0 library.
		// I am not supporting non-x86-64 branch servers, so this is OK and I don't need to
		// do any library scanning nonsense to pick the correct one.
		//
		// (note that the x86-64 branch also includes 32-bit binaries, rather
		// weirdly. You can build the module for this too, but not the non-x86-64 branch
		// 32-bit GMod, and it also shares having only one tier0 library.)
		tier0::library = lucore::Library::Open("tier0");
		tier0::Msg = tier0::library->Symbol<tier0::Msg_t>("Msg");
		tier0::Warning = tier0::library->Symbol<tier0::Msg_t>("Warning");
	}

	void SourceSink::OutputMessage(const lucore::Logger::MessageData& data) {
		auto formatted =
		std::format("[LCPU Native/{}] [{}] {}\n", lucore::Logger::SeverityToString(data.severity),
					data.time, std::vformat(data.format, data.args));
		if(data.severity < lucore::Logger::MessageSeverity::Warning) {
			tier0::Msg("%s", formatted.c_str());
		} else {
			tier0::Warning("%s", formatted.c_str());
		}
	}

} // namespace lcpu
