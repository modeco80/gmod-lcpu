#include <format>
#include <lucore/Assert.hpp>
#include <lucore/Logger.hpp>

/// Sample implementation of lucore logger sink.
struct MySink : public lucore::Logger::Sink {
	void OutputMessage(lucore::Logger::MessageSeverity sev, std::string_view fmt, std::format_args args) override {
		std::printf("[My Sink] [%s] %s\n", lucore::Logger::SeverityToString(sev).data(), std::vformat(fmt, args).c_str());
	}

	static MySink& The() {
		static MySink sink;
		return sink;
	}
};

int main() {
	lucore::LoggerAttachStdout();
	auto& logger = lucore::Logger::The();

	logger.AttachSink(MySink::The()); // attach our sink

	logger.Info("Hello World {}", 123.456);
	logger.Warning("sample warning");
	logger.Error("Smaple Error");

	LUCORE_CHECK(false, "should appear");
}
