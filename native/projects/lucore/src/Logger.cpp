#include <iostream>
#include <lucore/Logger.hpp>

namespace lucore {

	Logger& Logger::The() {
		static Logger logger;
		return logger;
	}

	void Logger::AttachSink(Sink& sink) {
		sinks.push_back(&sink);
	}

	void Logger::VOut(MessageSeverity severity, std::string_view format, std::format_args args) {
		// give up early if no sinks are attached
		if(sinks.empty())
			return;

		if(severity < logLevel)
			return;

		MessageData data { .time = std::chrono::system_clock::now(),
						   .severity = severity,
						   .format = format,
						   .args = args };

		for(auto sink : sinks)
			sink->OutputMessage(data);
	}

	StdoutSink& StdoutSink::The() {
		static StdoutSink sink;
		return sink;
	}

	void StdoutSink::OutputMessage(const Logger::MessageData& data) {
		// This is very nasty, but required until more standard libraries support the C++23 <print>
		// header.
		std::puts(std::format("[Lucore/{}] [{}] {}", Logger::SeverityToString(data.severity),
							  data.time, std::vformat(data.format, data.args))
				  .c_str());
	}

	void LoggerAttachStdout() {
		Logger::The().AttachSink(StdoutSink::The());
	}

} // namespace lucore
