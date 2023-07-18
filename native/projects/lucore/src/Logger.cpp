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

		MessageData data { .time = std::chrono::system_clock::now(),
						   .severity = severity,
						   .format = format,
						   .args = args };

		for(auto sink : sinks)
			sink->OutputMessage(data);
	}
#if 0
	StdoutSink& StdoutSink::The() {
		static StdoutSink sink;
		return sink;
	}

	void StdoutSink::OutputMessage(const Logger::MessageData& data) {
		// This is kinda iffy, but required until more standard libraries support the C++23 <print>
		// header.
		struct FileOutIterator {
			using iterator_category = std::output_iterator_tag;
			using value_type = void;
			using difference_type = std::ptrdiff_t;
			using pointer = void;
			using reference = void;

			FileOutIterator(std::FILE* file) : file(file) {}
			FileOutIterator& operator*() { return *this; }
			FileOutIterator& operator++() { return *this; }
			FileOutIterator& operator++(int) { return *this; }

			FileOutIterator& operator=(const char& val) {
				fputc(val, file);
				return *this;
			}

		   private:
			std::FILE* file;
		};
		std::format_to(
		FileOutIterator(data.severity < Logger::MessageSeverity::Error ? stdout : stderr),
		"[Lucore/{}] [{}] {}\n", Logger::SeverityToString(data.severity), data.time,
		std::vformat(data.format, data.args));
	}

	void LoggerAttachStdout() {
		Logger::The().AttachSink(StdoutSink::The());
	}
#endif

} // namespace lucore
