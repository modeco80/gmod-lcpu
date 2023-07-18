#include "SourceSink.hpp"

#include <lucore/Library.hpp>
#include <lucore/Types.hpp>

namespace tier0 {
	// TODO: For now this'll leak..
	lucore::Library* library = nullptr;

	/// Minimally Source compatiable Color type
	/// (Even more POD than Source)
	struct Color {
		constexpr explicit Color(u8 r, u8 g, u8 b, u8 a) {
			colors[0] = r;
			colors[1] = g;
			colors[2] = b;
			colors[3] = a;
		}

		u8 colors[4];
	};

	using LoggingChannelID_t = s32;
	enum LoggingSeverity_t {
		LS_MESSAGE,
		LS_WARNING,
		LS_ASSERT,
		LS_ERROR,

		LS_HIGHEST_SEVERITY
	};

	enum LoggingChannelFlags_t { LCF_CONSOLE_ONLY = 1, LCF_DO_NOT_ECHO = 2 };

	using RegisterTagsFunc = void (*)();

	// LoggingSystem_ APIs
	using RegisterLoggingChannel_t = LoggingChannelID_t (*)(const char*, RegisterTagsFunc,
															int flags, LoggingSeverity_t severity,
															Color color);
	using Log_t = s32 (*)(LoggingChannelID_t, LoggingSeverity_t, const char*, ...);
	using GetChannelColor_t = Color (*)(LoggingChannelID_t);

	RegisterLoggingChannel_t LoggingSystem_RegisterLoggingChannel {};
	Log_t LoggingSystem_Log {};
	GetChannelColor_t LoggingSystem_GetChannelColor {};

} // namespace tier0

namespace lcpu {

	tier0::LoggingChannelID_t gSourceSinkLoggerChannel { -1 };

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

#define GRAB_SYMBOL(name, T) tier0::name = tier0::library->Symbol<T>(#name);

		GRAB_SYMBOL(LoggingSystem_RegisterLoggingChannel, tier0::RegisterLoggingChannel_t);
		GRAB_SYMBOL(LoggingSystem_Log, tier0::Log_t);
		GRAB_SYMBOL(LoggingSystem_GetChannelColor, tier0::GetChannelColor_t);

		gSourceSinkLoggerChannel = tier0::LoggingSystem_RegisterLoggingChannel(
		"LCPU Native", nullptr, 0, tier0::LS_MESSAGE, tier0::Color(0xff, 0x99, 0x00, 0xff));
	}

	void SourceSink::OutputMessage(const lucore::Logger::MessageData& data) {
		auto formatted =
		std::format("[LCPU Native/{}] [{}] {}\n", lucore::Logger::SeverityToString(data.severity),
					data.time, std::vformat(data.format, data.args));
		auto lucore_to_source = [severity = data.severity]() -> tier0::LoggingSeverity_t {
			using enum lucore::Logger::MessageSeverity;
			switch(severity) {
				case Info:
					return tier0::LS_MESSAGE;
				case Warning:
					return tier0::LS_WARNING;
				case Error:
					return tier0::LS_WARNING;
				case Fatal:
					return tier0::LS_ASSERT;
				default:
					return tier0::LS_MESSAGE;
			}
		}();

#if 0
		// This is a pretty stupid hack for console colors on Linux.
		// I don't really like it that much tbh, wish tier0 supported this.
		auto color = tier0::LoggingSystem_GetChannelColor(gSourceSinkLoggerChannel);

		{
			using enum lucore::Logger::MessageSeverity;
			switch(data.severity) {
				case Warning:
					color = tier0::Color(0xff, 0xff, 0x0, 0xff);
					break;
				case Error:
				case Fatal:
					color = tier0::Color(0xff, 0x0, 0x0, 0xff);
					break;
				default:
					break;
			}
		}

		tier0::LoggingSystem_Log(gSourceSinkLoggerChannel, lucore_to_source,
								 "\033[38;2;%d;%d;%dm%s\033[0m", color.colors[0], color.colors[1],
								 color.colors[2], formatted.c_str());
#else
		tier0::LoggingSystem_Log(gSourceSinkLoggerChannel, lucore_to_source, "%s",
								 formatted.c_str());
#endif
	}

} // namespace lcpu
