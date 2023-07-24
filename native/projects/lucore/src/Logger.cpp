#include <bits/iterator_concepts.h>

#include <cstddef>
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

		MessageData data { .time = std::chrono::system_clock::now(), .severity = severity, .format = format, .args = args };

		for(auto sink : sinks)
			sink->OutputMessage(data);
	}

} // namespace lucore
