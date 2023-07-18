#include <lucore/StdoutSink.hpp>

namespace lucore {
	StdoutSink& StdoutSink::The() {
		static StdoutSink sink;
		return sink;
	}

	void StdoutSink::OutputMessage(const Logger::MessageData& data) {
		// This is kinda iffy, but required until more standard libraries support the C++23 <print>
		// header.
		struct FputcIterator {
			using iterator_category = std::output_iterator_tag;
			using value_type = void;
			using difference_type = std::ptrdiff_t;
			using pointer = void;
			using reference = void;

			FputcIterator(std::FILE* file) : file(file) {}
			FputcIterator& operator*() { return *this; }
			FputcIterator& operator++() { return *this; }
			FputcIterator& operator++(int) { return *this; }

			FputcIterator& operator=(const char& val) {
				fputc(val, file);
				return *this;
			}

		   private:
			std::FILE* file;
		};

		auto it = FputcIterator(data.severity < Logger::MessageSeverity::Error ? stdout : stderr);
		std::format_to(it, "[Lucore/{}] [{}] {}\n", Logger::SeverityToString(data.severity),
					   data.time, std::vformat(data.format, data.args));
	}

	void LoggerAttachStdout() {
		Logger::The().AttachSink(StdoutSink::The());
	}
} // namespace lucore
