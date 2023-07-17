#include <format>
#include <lucore/Logger.hpp>


namespace lucore {

	Logger& Logger::The() {
		static Logger logger;
		return logger;
	}

	void Logger::AttachSink(Sink& sink) {
		sinks[sinkCount++] = &sink;
	}

	void Logger::VOut(MessageSeverity severity, std::string_view format, std::format_args args) {
		if(severity < logLevel)
			return;

		for(int i = 0; i < sinkCount; ++i)
			sinks[i]->OutputMessage(severity, format, args);
	}

	StdoutSink& StdoutSink::The() {
		static StdoutSink sink;
		return sink;
	}

	void StdoutSink::OutputMessage(Logger::MessageSeverity severity, std::string_view format, std::format_args args) {
		auto message = std::vformat(format, args);
		std::printf("[Lucore Stdout/%s] %s\n", Logger::SeverityToString(severity).data(), message.c_str());
	}

	void LoggerAttachStdout() {
		Logger::The().AttachSink(StdoutSink::The());
	}

}
