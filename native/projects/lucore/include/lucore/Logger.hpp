//! Logging utilities for Lucore
//! Using Standard C++ <format>

#include <format>
#include <cstdint>

namespace lucore {

	/// The Lucore logger.
	struct Logger {
		enum class MessageSeverity {
			Debug,
			Info,
			Warning,
			Error,
			Fatal
		};

		static constexpr std::string_view SeverityToString(MessageSeverity sev) {
			const char* table[] = {
				"Deb",
				"Inf",
				"Wrn",
				"Err",
				"Ftl"
			};
			return table[static_cast<std::size_t>(sev)];
		}

		/// A sink.
		struct Sink {
			virtual void OutputMessage(MessageSeverity severity, std::string_view format, std::format_args args) = 0;
		};


		static Logger& The();

		Logger() = default;
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;

		void AttachSink(Sink& sink);

		MessageSeverity GetLogLevel() const {
			return logLevel;
		}

		void SetLogLevel(MessageSeverity newLogLevel) {
			logLevel = newLogLevel;
		}

		template<class... Args>
		inline void Debug(std::string_view fmt, Args... args) {
			VOut(MessageSeverity::Debug, fmt, std::make_format_args(std::forward<Args>(args)...));
		}

		template<class... Args>
		inline void Info(std::string_view fmt, Args... args) {
			VOut(MessageSeverity::Info, fmt, std::make_format_args(std::forward<Args>(args)...));
		}
		
		template<class... Args>
		inline void Warning(std::string_view fmt, Args... args) {
			VOut(MessageSeverity::Warning, fmt, std::make_format_args(std::forward<Args>(args)...));
		}

		template<class... Args>
		inline void Error(std::string_view fmt, Args... args) {
			VOut(MessageSeverity::Error, fmt, std::make_format_args(std::forward<Args>(args)...));
		}

		template<class... Args>
		inline void Fatal(std::string_view fmt, Args... args) {
			VOut(MessageSeverity::Fatal, fmt, std::make_format_args(std::forward<Args>(args)...));
		}

	private:
		void VOut(MessageSeverity severity, std::string_view format, std::format_args args);

		MessageSeverity logLevel{MessageSeverity::Info};

		Sink* sinks[4];
		std::uint8_t sinkCount;
	};

	/// A logger sink implementation that prints to standard output.
	struct StdoutSink : public Logger::Sink {
		static StdoutSink& The();

		virtual void OutputMessage(Logger::MessageSeverity severity, std::string_view format, std::format_args args) override;
	};

	/// Attach stdout to the logger.
	void LoggerAttachStdout();

}
