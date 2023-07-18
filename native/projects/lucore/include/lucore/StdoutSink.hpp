#include <lucore/Logger.hpp>

namespace lucore {

	/// A logger sink implementation that prints to standard output.
	/// Not used by the lcpu native module, but could be used by applications
	/// using lucore later on.
	struct StdoutSink : public Logger::Sink {
		static StdoutSink& The();

		virtual void OutputMessage(const Logger::MessageData& data) override;
	};

	/// Attach the stdout logger sink to the logger.
	void LoggerAttachStdout();

}
